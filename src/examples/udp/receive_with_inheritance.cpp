#include <QCoreApplication>
#include <QDebug>
#include <QCommandLineParser>
#include <greeting.h>
#include <roll_dice.h>
#include <speech/udp/udp_receiver.h>

struct my_receiver : speech::udp::udp_receiver<greeting, roll_dice> {

     explicit my_receiver ( speech::port p )
        : speech::udp::udp_receiver<greeting, roll_dice> { p }
    { }

protected:

     void on_receive ( const greeting& greeting ) override
     {
          qDebug() << "receive => " << greeting;
     }

     void on_receive ( const roll_dice& dice ) override
     {
          qDebug() << "receive => " << dice;
     }

};

int main ( int argc, char** argv )
{
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


    my_receiver receiver{ speech::port{ port } };
    qDebug() << "Moving";
    auto moved_received = std::move(receiver);
    qDebug() << "Moved";
//    my_receiver receiver{ speech::port{port} };

     qDebug() << "Udp port " << port << " is listening.";

     return app.exec();
}
