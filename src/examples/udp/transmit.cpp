#include <QDataStream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include <QCommandLineParser>
#include <memory>
#include <greeting.h>
#include <roll_dice.h>
#include <speech/speech.h>

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
     auto unique_sck = make_unique<QUdpSocket>();
     udp_transmitter<greeting, roll_dice> udp{ host, speech::port ( port ) };
     udp_transmitter<greeting, roll_dice> udp_shared{ shared_sck, host, speech::port ( port ) };
     udp_transmitter<greeting, roll_dice> udp_unique{ std::move ( unique_sck ), host, speech::port ( port ) };

    for (int i = 0;; ++i)
    {
        greeting greeting_msg;
        greeting_msg.my_name_is = QString("%0 - %1").arg("Hi ! My name is Ozan -").arg(i);
        greeting_msg.numbers.push_back(1);
        greeting_msg.numbers.push_back(2);
        greeting_msg.numbers.push_back(3);

        udp.transmit(greeting_msg);
        udp_shared.transmit(greeting_msg);
        udp_unique.transmit(greeting_msg);

          qDebug() << "transmitting to " << port << " => " << greeting_msg;
          QCoreApplication::processEvents();
          QThread::msleep ( rand() % 1000 );

          roll_dice dice;
          udp.transmit ( dice );
          udp_shared.transmit ( dice );
          udp_unique.transmit ( dice );

          qDebug() << "transmitting to " << port << " => " << dice;
          QCoreApplication::processEvents();
          QThread::msleep ( rand() % 1000 );
     }
}
