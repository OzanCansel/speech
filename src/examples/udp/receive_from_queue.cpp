#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <QCommandLineParser>
#include <speech/udp/udp_receiver.h>
#include <greeting.h>
#include <roll_dice.h>


int main ( int argc, char **argv )
{

     using namespace speech::udp;

     QCoreApplication app ( argc, argv );

     QCommandLineParser parser;
     parser.addHelpOption();

     parser.addOptions ( {{ {"p", "port"}, "Specify port number", "port number" }} );

     parser.process ( app );

     //Defaults
     auto port = 24942;

     if ( parser.isSet ( "p" ) ) {
          port = parser.value ( "p" ).toInt();
     }

     queued_udp_receiver<greeting, roll_dice> udp{ speech::port ( port ) };

     QTimer checkMessages;

     checkMessages.setInterval ( 30 );
     QObject::connect ( &checkMessages, &QTimer::timeout, [&]() {

          while ( !udp.messages<roll_dice>().empty() ) {
               qDebug() << "roll_dice => " << udp.messages<roll_dice>().front();
               udp.messages<roll_dice>().pop();
          }

          while ( !udp.messages<greeting>().empty() ) {
               qDebug() << "greeting => " << udp.messages<greeting>().front();
               udp.messages<greeting>().pop();
          }

     } );

     checkMessages.start();

     qDebug() << "Udp port " << port << " is listening.";

     return QCoreApplication::exec();
}
