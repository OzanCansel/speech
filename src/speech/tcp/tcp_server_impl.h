#include "speech/error/connection_error.h"
#include "speech/error/port_in_use_error.h"
#include "speech/util.h"
#include "tcp_server.h"
#include <QObject>
#include <utility>

namespace speech
{
namespace tcp
{

tcp_server::tcp_server ( const QHostAddress &address, speech::port p )
{
    using namespace speech::error;

    if ( !m_server.listen ( address, p.get() ) ) {
        if ( m_server.serverError() == QAbstractSocket::AddressInUseError )
            throw port_in_use_error{ m_server.errorString().toStdString() };

        throw std::runtime_error { m_server.errorString().toStdString() };
    }

    QObject::connect ( &m_server, &QTcpServer::newConnection, [this] { new_connection(); } );
}

void tcp_server::new_connection()
{
    //Fork new receiver and push to receivers list
    auto socket = m_server.nextPendingConnection();

    m_alive_connections.emplace_back( std::ref( *socket ) );

    m_alive_connections.back().attach( std::bind( &tcp_server::ready_read_callback , this ,
                                                  std::placeholders::_1 ,
                                                  std::placeholders::_2 ) );

    QObject::connect ( socket, &QTcpSocket::disconnected, [ this, socket ]() {
        disconnected ( socket );
    });
}

void tcp_server::disconnected ( QTcpSocket* socket )
{
    auto entry = std::find_if ( m_alive_connections.begin(), m_alive_connections.end(), [&socket] ( auto& shared_socket ) {
        return &shared_socket.socket() == socket;
    } );

    if ( entry != m_alive_connections.end() ) {
        m_alive_connections.erase ( entry );
    }

}

int tcp_server::ready_read_callback( const QByteArray& data , QTcpSocket& sck )
{

    using namespace std;

    auto max_res = -9999;
    vector<shared_ptr<impl::lifetime>> ended_lifetimes;

    for ( auto& f : m_lifetimes )
    {

        if ( !f )
        {
            ended_lifetimes.push_back( f );
            continue;
        }

        if ( f.use_count() == 1 )
        {
            ended_lifetimes.push_back( f );
            continue;
        }

        auto res = f->cb( data , sck );

        // Bad should be refactored
        if ( res == -2 )
            return -2;

        if( res > max_res )
            max_res = res;
    }

    // Remove ended lifetimes
    for ( auto& life : ended_lifetimes  )
        m_lifetimes.erase( std::remove( m_lifetimes.begin() , m_lifetimes.end() ,  life ) );

    return max_res;
}
}
}
