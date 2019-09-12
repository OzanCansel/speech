#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <QCommandLineParser>
#include <speech/tcp/tcp_server.h>
#include "greeting.h"
#include "roll_dice.h"

int main ( int argc, char** argv )
{
     QCoreApplication app ( argc, argv );

     using namespace speech;
     using namespace speech::tcp;
     using namespace speech::impl;

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

     qDebug() << "Tcp Server running at " << port << " port";

     app.exec();
}
