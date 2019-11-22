#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_transmitter.h>
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

    parser.addOptions ( {
         {{"p", "port"}, "Specify host port number", "port number" } ,
         {{"a", "addr"}, "Specify host address", "host_addr" }
    } );

    parser.process ( app );

    //Defaults
    auto port{ 24942 };

    QHostAddress host ( QHostAddress::LocalHost );

    if ( parser.isSet ( "p" ) ) {
         port = parser.value ( "p" ).toInt();
    }

    if ( parser.isSet ( "a" ) ) {
         host = QHostAddress ( parser.value ( "a" ) );
    }

    auto sck = std::make_shared< QTcpSocket >();

    sck->connectToHost( host , port );

    if ( !sck->waitForConnected() )
        throw std::runtime_error { std::string { "Could not connect to " } + host.toString().toStdString() + std::string { ":" } + std::to_string( port ) };

    auto res = ask<yes , no>( u_there{} , sck , std::chrono::milliseconds { 5000 } );

    if ( responded_with< yes > ( res ) )
    {
        qDebug() << "He says yes";
    }

    if ( responded_with< no > ( res ) )
    {
        qDebug() << "He says no";
    }


    constexpr int loop_c = 10;
    auto start = std::chrono::high_resolution_clock::now();
    for ( auto i = 0; i < loop_c; i++ )
    {
        auto roll = ask( roll_a_dice {} , sck , std::chrono::milliseconds { 50 } );
        qDebug() << roll;
    }

    auto time_in_ms = static_cast<double> ( std::chrono::duration_cast<std::chrono::microseconds>( std::chrono::high_resolution_clock::now() - start ).count() );
    auto fps = 1000000.0 / ( time_in_ms / loop_c );

    qDebug() << "FPS : " << fps;

    return QCoreApplication::exec();
}
