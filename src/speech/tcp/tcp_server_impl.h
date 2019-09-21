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

namespace impl
{

template <typename Tuple, typename F, std::size_t ...Indices>
inline void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1,
                  (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
                 };
}

template <typename Tuple, typename F>
inline void for_each(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

}

template<typename T>
handler<T>::handler( client_cb&& cb , std::shared_ptr<impl::lifetime> c )
    : m_conn { std::move( c ) }
    , m_cb { std::forward<client_cb>( cb ) }
{
    using namespace std::placeholders;
    m_conn->cb = std::bind( &tcp_receiver<T>::on_data_received , this , _1 , _2 );
}

template<typename T>
handler<T>::handler( handler<T>&& other ) noexcept
{
    using namespace std::placeholders;

    m_conn = other.m_conn;
    m_cb = other.m_cb;
    m_conn->cb = std::bind( &tcp_receiver<T>::on_data_received , this , _1 , _2 );
}

template<typename T>
handler<T>& handler<T>::operator = ( handler&& other ) noexcept
{
    using namespace std::placeholders;

    m_conn = other.m_conn;
    m_cb = other.m_cb;
    m_conn->cb = std::bind( tcp_receiver<T>::on_data_received , this , _1 , _2 );
}

template <typename T>
void handler<T>::on_receive( const T& e )
{
    m_cb( e , tcp_receiver<T>::socket() );
}

inline tcp_server::tcp_server ( const QHostAddress &address, speech::port p )
{
    using namespace speech::error;

    if ( !m_server.listen ( address, p.get() ) ) {
        if ( m_server.serverError() == QAbstractSocket::AddressInUseError )
            throw port_in_use_error{ m_server.errorString().toStdString() };

        throw std::runtime_error { m_server.errorString().toStdString() };
    }

    QObject::connect ( &m_server, &QTcpServer::newConnection, [this] { new_connection(); } );
}

inline void tcp_server::new_connection()
{
    //Fork new receiver and push to receivers list
    auto socket = m_server.nextPendingConnection();

    m_alive_connections.emplace_back( std::shared_ptr< QTcpSocket > { socket , socket_deleter {} } );

    m_alive_connections.back().attach( std::bind( &tcp_server::ready_read_callback , this ,
                                                  std::placeholders::_1 ,
                                                  std::placeholders::_2 ) );

    QObject::connect ( socket, &QTcpSocket::disconnected, [ this, socket ]() {
        disconnected ( socket );
    });
}

inline void tcp_server::disconnected ( QTcpSocket* socket )
{
    auto entry = std::find_if ( m_alive_connections.begin(), m_alive_connections.end(), [&socket] ( auto& shared_socket ) {
        return shared_socket.socket().lock().get() == socket;
    } );

    if ( entry != m_alive_connections.end() ) {
        m_alive_connections.erase ( entry );
    }

}

inline int tcp_server::ready_read_callback( const QByteArray& data , std::weak_ptr<QTcpSocket> sck )
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

inline int tcp_server::port() const
{
    return m_server.serverPort();
}

template<typename... T>
inline tcp_server& tcp_server::listen( const std::tuple<handler<T>...>& handlers )
{
    impl::for_each( handlers , [ this ]( auto& handler ) { m_lifetimes.push_back( handler.m_conn ); } );

    return *this;
}

template<typename T>
inline tcp_server& tcp_server::listen( const handler<T>& handler )
{
    m_lifetimes.push_back( handler.m_conn );
    return *this;
}

}
}
