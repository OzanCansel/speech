#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QTimer>
#include <speech/udp/udp_receiver.h>
#include <speech/qobject_serialization.h>
#include "entity.h"
#include "car_info.h"

struct qobject_receiver : speech::udp::udp_receiver<entity, car_info> {

     qobject_receiver ( int port ) : speech::udp::udp_receiver<entity, car_info>
     {
          speech::port ( port )
     } { }

protected:

     void on_receive ( const entity& greeting ) override
     {
          using namespace speech;

          qDebug() << "receive => " << greeting;
     }

     void on_receive ( const car_info& car ) override
     {
          using namespace speech;

          qDebug() << "receive => " << car;
     }
};

int main ( int argc, char **argv )
{

     using namespace speech;
     using namespace speech::udp;

     QCoreApplication app ( argc, argv );


     QCommandLineParser parser;
     parser.addHelpOption();

     parser.addOptions ( {
          { {"p", "port"}, "Specify listening port", "port number" }
     } );

     parser.process ( app );

     //Defaults
     auto port = 24942;

     if ( parser.isSet ( "p" ) ) {
          port = parser.value ( "p" ).toInt();
     }

     qDebug() << "Starting to listen udp port " << port;

     qobject_receiver receiver { port };

     return app.exec();
}
