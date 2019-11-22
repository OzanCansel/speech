#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <speech/speech.h>
#include <speech/tcp/tcp_ask.h>
#include <QBuffer>

using namespace speech::serialization::container;

struct u_there
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

TEST_CASE( "ask test" , "[ask]" )
{
    using namespace speech;
    using namespace speech::tcp;

//    QTcpSocket sck;
//    auto res = ask< yes , no >( u_there{} , sck );

//    if ( responded_with< yes > ( res ) )
//    {
//        qDebug() << "Responded with yes";
//    }

//    if ( responded_with< no > ( res ))
//    {
//        qDebug() << "Responded with no";
//    }

}
