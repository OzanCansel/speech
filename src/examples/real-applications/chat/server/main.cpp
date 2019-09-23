#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QUuid>
#include <QTimer>
#include <QDebug>
#include <map>
#include <speech/speech.h>
#include <messages.h>

int main( int argc , char** argv )
{
    using namespace speech;
    using namespace speech::tcp;

    QCoreApplication app( argc , argv );

    QCommandLineParser parser;
    QCommandLineOption port_opt { { "p" , "port" } , "Listening port" , "port" , "24942" };
    parser.addHelpOption();
    parser.addOption( port_opt );

    parser.process( app );

    auto port = parser.value( port_opt ).toInt();

    if ( port <= 0 )
    {
        qDebug() << "Port cannot be negative or zero.";
        return 1;
    }

    tcp_server game_server { QHostAddress::Any , speech::port { port } };

    std::map<std::shared_ptr<QTcpSocket> , QString> socket_to_username;

    auto accepted_messages =
            listen<let_me_join> ( [ &game_server , &socket_to_username ]( const let_me_join& e , std::weak_ptr<QTcpSocket> sck )
    {

        bool nick_name_exist = std::any_of( socket_to_username.begin() , socket_to_username.end() , [ &e ]( const auto& p ) { return p.second == e.nick_name ;});

        if ( nick_name_exist )
        {
            qDebug() << e << "exists.";
            transmit( chat_error { "nick_name exists." } , sck.lock() );
            game_server.disconnect_socket( sck );
            return;
        }

        qDebug() << "New user joined. " << e ;

        for ( const auto& p : socket_to_username )
            transmit( new_user_joined { p.second } , sck.lock() );

        socket_to_username.insert( { sck.lock() , e.nick_name } );

        game_server.broadcast( new_user_joined{ e.nick_name } );

    }) |
            listen<send_message>( [ &game_server , &socket_to_username ] ( const send_message& m , std::weak_ptr<QTcpSocket> sck )
    {
        try {
            auto user_name = socket_to_username.at( sck.lock() );
            qDebug() << user_name << m;
            game_server.broadcast( new_message { m.message , user_name });
        } catch ( const std::out_of_range& ) {
            game_server.disconnect_socket( sck );
            qDebug() << "User could not found.";
        }

    });
    QTimer check_disconnected_sockets;
    check_disconnected_sockets.setInterval( 500 );

    QObject::connect( &check_disconnected_sockets , &QTimer::timeout , [ &socket_to_username , &game_server ] () {

        for ( auto it = socket_to_username.begin(); it != socket_to_username.end(); )
        {
            std::shared_ptr<QTcpSocket> socket = it->first;
            auto user_name = it->second;

            if ( socket->state() != QTcpSocket::ConnectedState )
            {
                game_server.broadcast( disconnected { user_name } );
                qDebug() << user_name << "disconnected";
                socket_to_username.erase( it++ );
            }
            else
            {
                it++;
            }
        }
    });

    check_disconnected_sockets.start();

    game_server.listen( accepted_messages );

    qDebug() << "chat server initialized at" << game_server.port() << "port";

    return QCoreApplication::exec();
}
