#include <QCoreApplication>
#include <QUuid>
#include <QDebug>
#include <map>
#include <speech/speech.h>
#include <messages.h>

int main( int argc , char** argv )
{
    using namespace speech;
    using namespace speech::tcp;

    QCoreApplication app( argc , argv );

    tcp_server game_server { QHostAddress::Any , speech::port { 24942 } };

    std::map<std::shared_ptr<QTcpSocket> , QString> socket_to_username;

    auto accepted_messages =
            listen<let_me_join> ( [ &game_server , &socket_to_username ]( const let_me_join& e , std::weak_ptr<QTcpSocket> sck )
    {
        qDebug() << "New user joined. " << e ;

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
            qDebug() << "User could not found.";
        }

    });

    game_server.listen( accepted_messages );

    qDebug() << "chat server initialized at" << game_server.port() << "port";

    return QCoreApplication::exec();
}
