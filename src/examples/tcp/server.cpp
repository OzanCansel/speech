#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/tcp_receiver.h>
#include <greeting.h>
#include <roll_dice.h>

struct tcp_server_redirects_entities_to_me
{
    void operator()( const greeting& g , std::weak_ptr<QTcpSocket> ) { qDebug() << "Redirected " << g; }
    void operator()( const roll_dice& r , std::weak_ptr<QTcpSocket> ) { qDebug() << "Redirected " << r; }
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

    tcp_server server { QHostAddress::Any , speech::port { port } };

    tcp_server_redirects_entities_to_me subscriber;

    auto listeners =
            redirect< greeting , roll_dice >( subscriber ) |
            listen( []( const roll_dice& e , std::weak_ptr<QTcpSocket> ) {
        qDebug() << "Received : " << e;
    }) |
            listen( [] ( const greeting& g , std::weak_ptr<QTcpSocket> ){
        qDebug() << "Received : " << g;
    }) |    listen( [] ( const greeting& g , std::weak_ptr<QTcpSocket> ){

    });

    server.listen( listeners );

    qDebug() << "Tcp Server running at " << port << " port";

    return QCoreApplication::exec();
}
