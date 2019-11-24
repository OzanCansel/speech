#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/tcp_receiver.h>
#include <speech/tcp/tcp_ask.h>
#include <speech/speech.h>
#include "questions.h"

int main ( int argc, char** argv )
{
    QCoreApplication app ( argc, argv );

    using namespace speech;
    using namespace speech::tcp;

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions (
    {{ {"p", "port"}, "Specify listening port number", "port number" }} );

    parser.process ( app );

    //Defaults
    auto port = 24942;

    if ( parser.isSet( "p" ) ) {
        port = parser.value( "p" ).toInt();
    }

    tcp_server server { QHostAddress::Any , speech::port { port } };

    auto listeners =
            listen( []( const u_there& , std::weak_ptr< QTcpSocket > sck ) {
        if ( std::rand() % 2 == 0 )
            transmit( yes{} , sck.lock() );
        else
            transmit( no{} , sck.lock() );
    }) |
            listen( [](const roll_a_dice& , std::weak_ptr< QTcpSocket > sck ) {
        transmit( response_of< roll_a_dice > { std::rand() % 100 } , sck.lock() );
    });

    server.listen( listeners );

    qDebug() << "Tcp Server running at " << port << " port";
    qDebug() << "I will respond to 'roll_a_dice' and 'u_there' questions.";

    return QCoreApplication::exec();
}
