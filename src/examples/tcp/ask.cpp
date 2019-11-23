#include <QCoreApplication>
#include <QUuid>
#include <QCommandLineParser>
#include <QDebug>
#include <speech/tcp/tcp_transmitter.h>
#include <speech/tcp/tcp_ask.h>
#include <speech/speech.h>
#include "questions.h"

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

    auto res = ask<yes , no> ( u_there{} , sck );

    qDebug().noquote() << QString( 80 , '=' );

    if ( responded_with< yes > ( res ) )
        qDebug() << "He says yes";

    if ( responded_with< no > ( res ) )
        qDebug() << "He says no";

    qDebug().noquote() << QString( 80 , '=' );

    qDebug() << "Roll a dice ten times";

    qDebug().noquote() << QString( 80 , '=' );

    std::vector< int > chances;

    for ( auto i = 0; i < 10; i++ )
    {
        auto roll = ask( roll_a_dice {} , sck );
        qDebug() << roll;

        chances.push_back( roll.luck );
    }

    auto average = std::accumulate( chances.begin() , chances.end() , 0 ) / chances.size();

    qDebug().noquote() << QString( 80 , '=' );

    if ( average < 40 )
        qDebug() << "It seems you are not lucky today.";
    else if ( average >= 40 && average <= 60 )
        qDebug() << "Not bad at all";
    else
        qDebug() << "Wow, very good";

    qDebug().noquote() << QString( 80 , '=' );

    return QCoreApplication::exec();
}
