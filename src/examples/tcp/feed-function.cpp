#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QCommandLineParser>
#include <speech/tcp/tcp_receiver.h>
#include <speech/tcp/tcp_transmitter.h>
#include <memory>
#include <greeting.h>
#include <roll_dice.h>
#include <me.h>

int main ( int argc, char** argv )
{
    using namespace speech;
    using namespace speech::tcp;

    QCoreApplication app ( argc, argv );


    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions ( {
                            {{"p", "port"}, "Specify host port number", "port number" },
                            {{"a", "addr"}, "Specify host address", "frame per second" },
                            {{"f", "fps"}, "Specify transmitting rate", "frame per second" }
                        } );

    parser.process ( app );

    //Defaults
    auto port{ 24942 };
    auto fps{ 2 };
    QHostAddress host ( QHostAddress::LocalHost );

    if ( parser.isSet ( "p" ) ) {
        port = parser.value ( "p" ).toInt();
    }

    if ( parser.isSet ( "f" ) ) {
        fps = parser.value ( "f" ).toInt();
    }

    if ( parser.isSet ( "a" ) ) {
        host = QHostAddress ( parser.value ( "a" ) );
    }

    auto delay = 1000.0 / fps;

    auto shared_sck = std::shared_ptr<QTcpSocket>{ new QTcpSocket{} , socket_deleter{} };

    QTcpSocket socket;
    QTcpSocket already_opened_sck;
    already_opened_sck.connectToHost ( host, static_cast<quint16>( port ) );

    if ( !already_opened_sck.waitForConnected() ) {
        throw std::runtime_error ( "Could not connect to host" );
    }

    for ( auto i = 0;; ++i ) {
        greeting g;

        g.my_name_is = QString ( "Hello %0" ).arg ( i );

        //Create a socket, connect to host, transmit data
        transmit ( g, host, speech::port ( port ) );

        //Use shared_sck, connect to host, transmit data
        transmit ( g, host, speech::port ( port ), shared_sck );

        //Use 'socket', connect to host and transmit
        transmit ( g, host, speech::port ( port ), socket );

        //transmit over already connected socket
        transmit ( g, std::ref ( already_opened_sck ) );
        transmit ( g, shared_sck );

        qDebug() << "transmit => " << g;

        QCoreApplication::processEvents();
        QThread::msleep ( static_cast<unsigned long>( delay ) );
    }
}
