#include <QDataStream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include <QCommandLineParser>
#include <speech/udp/udp_transmitter.h>
#include <memory>
#include <greeting.h>
#include <roll_dice.h>

int main ( int argc, char **argv )
{
     using namespace speech::udp;
     using namespace std;

     QCoreApplication app ( argc, argv );

     QCommandLineParser parser;
     parser.addHelpOption();

     parser.addOptions ( {
          { {"p", "port"}, "Specify port number", "port number" },
          { {"a", "addr"}, "Specify address ip", "address" }
     }
                       );

     parser.process ( app );

     //Defaults
     auto port = 24942;
     QHostAddress host{ QHostAddress::LocalHost };

     if ( parser.isSet ( "p" ) ) {
          port = parser.value ( "p" ).toInt();
     }

     if ( parser.isSet ( "a" ) )
          host = QHostAddress{ parser.value ( "a" ) };

     auto shared_sck = make_shared<QUdpSocket>();

     QUdpSocket socket;

     for ( int i = 0;; ++i ) {
          greeting greeting_msg;
          greeting_msg.my_name_is = QString ( "%0 - %1" ).arg ( "Hi ! My name is Ozan -" ).arg ( i );

          udp_transmit ( greeting_msg, host, speech::port ( port ) );
          udp_transmit ( greeting_msg, host, speech::port ( port ), std::ref ( socket ) );
          udp_transmit ( greeting_msg, host, speech::port ( port ), shared_sck );

          qDebug() << "transmitting to " << port << " => " << greeting_msg;
          QCoreApplication::processEvents();
          QThread::msleep ( rand() % 1000 );

          roll_dice dice;
          udp_transmit ( dice, host, speech::port ( port ) );
          udp_transmit ( dice, host, speech::port ( port ), std::ref ( socket ) );
          udp_transmit ( dice, host, speech::port ( port ), shared_sck );

          qDebug() << "transmitting to " << port << " => " << dice;
          QCoreApplication::processEvents();
          QThread::msleep ( rand() % 1000 );
     }
}
