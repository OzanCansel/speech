#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/tcp_receiver.h>
#include <greeting.h>
#include <roll_dice.h>

class handler_1 : public speech::tcp::tcp_receiver<greeting, roll_dice>
{

public:

     explicit handler_1 ( speech::shared_socket<QTcpSocket>& sck ) : speech::tcp::tcp_receiver<greeting, roll_dice>
     {
          sck
     }
     {
          qDebug() << "Handler_1 constructed. " << id.toString();
     }

     ~handler_1() override
     {
          qDebug() << "Handler_1 destroyed. " << id.toString();
     }

protected:

     void on_receive ( const greeting& greeting ) override
     {
          qDebug() << greeting;
     }

     void on_receive ( const roll_dice& dice ) override
     {
          qDebug() << dice;
     }

private:

     QUuid id { QUuid::createUuid() };

};


class handler_2 : public speech::tcp::tcp_receiver<roll_dice>
{

public:

     explicit handler_2 ( speech::shared_socket<QTcpSocket>& sck ) : speech::tcp::tcp_receiver<roll_dice>
     {
          sck
     }
     {
          qDebug() << "Handler_2 constructed. " << id.toString();
     }

     ~handler_2() override
     {
          qDebug() << "Handler_2 destroyed. " << id.toString();
     }

protected:

     void on_receive ( const roll_dice& dice ) override
     {
          qDebug() << dice;
     }

private:

     QUuid id { QUuid::createUuid() };

};

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

     auto sw = make_server ( QHostAddress::Any, speech::port{ port },
                             make_handler<handler_1>(),
                             make_handler<handler_2>() );

     qDebug() << "Tcp Server running at " << port << " port";

     return app.exec();
}
