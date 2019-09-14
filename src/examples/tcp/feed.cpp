#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QCommandLineParser>
#include <QString>
#include <QTcpSocket>
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

     auto delay = 1000.0 / fps / 2.0;

     auto shared_sck = std::make_shared<QTcpSocket>();
     auto unique_sck = std::make_unique<QTcpSocket>();

     tcp_transmitter<greeting, roll_dice, me> tcp { host, speech::port ( port ) };
     tcp_transmitter<greeting, roll_dice, me> tcp_unique { std::move ( unique_sck ) , host, speech::port ( port ) };
     tcp_transmitter<greeting, roll_dice, me> tcp_shared { shared_sck , host , speech::port ( port ) };

     for ( auto i = 0;; ++i ) {
          greeting g;

          g.my_name_is = QString ( "Hello %0" ).arg ( i );
          tcp.transmit ( g );
          tcp_unique.transmit ( g );
          tcp_shared.transmit ( g );
          qDebug() << "transmit => " << g;

          QThread::msleep ( delay );
          QCoreApplication::processEvents();

          roll_dice dice;

          dice.chance = rand() % 100;
          \
          tcp.transmit ( dice );
          tcp_unique.transmit ( dice );
          tcp_shared.transmit ( dice );

          qDebug() << "transmit => " << dice;

          tcp.transmit ( me{} );

          QCoreApplication::processEvents();
          QThread::msleep ( delay );
     }

     return app.exec();
}
