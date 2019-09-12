#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/tcp_receiver.h>
#include <greeting.h>
#include <roll_dice.h>

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

     if ( parser.isSet ( "p" ) ) {
          port = parser.value ( "p" ).toInt();
     }

     tcp_server server { QHostAddress::Any , speech::port { port } };

     auto res = listen<roll_dice>( []( const roll_dice& e , QTcpSocket& ) {
         qDebug() << "Received : " << e;
     }) |
             listen<greeting>( [] ( const greeting& g , QTcpSocket& ){
        qDebug() << "Received : " << g;
     }) |    listen<greeting>( [] ( const greeting& g , QTcpSocket& ){
         qDebug() << "Received : " << g;
      }) |   listen<greeting>( [] ( const greeting& g , QTcpSocket& ){
        qDebug() << "Received : " << g;
     });

     qDebug() << "Tcp Server running at " << port << " port";

     return app.exec();
}
