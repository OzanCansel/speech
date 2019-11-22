#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/tcp/tcp_receiver.h>
#include <speech/tcp/tcp_ask.h>
#include <speech/speech.h>

struct u_there
{
    SPEECH_SERIALIZE_EMPTY
};

struct roll_a_dice
{
  SPEECH_SERIALIZE_EMPTY
};

struct yes
{
    SPEECH_SERIALIZE_EMPTY
};

struct no
{
    SPEECH_SERIALIZE_EMPTY
};

template<>
struct speech::tcp::response_of< roll_a_dice >
{
    int luck {};

    SPEECH_SERIALIZE( luck )
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

    if ( parser.isSet( "p" ) ) {
        port = parser.value( "p" ).toInt();
    }

    tcp_server server { QHostAddress::Any , speech::port { port } };

    auto listeners =
            listen< u_there >( []( const u_there& , std::weak_ptr< QTcpSocket > sck )
    {
        if ( std::rand() % 2 == 0 )
            transmit( yes{} , sck.lock() );
        else
            transmit( no{} , sck.lock() );
    }) |
            listen< roll_a_dice >( [](const roll_a_dice& , std::weak_ptr< QTcpSocket > sck )
    {
        transmit( response_of< roll_a_dice > { std::rand() % 100 } , sck.lock() );
    });

    server.listen( listeners );

    qDebug() << "Tcp Server running at " << port << " port";

    return QCoreApplication::exec();
}
