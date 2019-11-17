#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <speech/speech.h>
#include <speech/serialize/container_serialize.h>
#include <QBuffer>

using namespace speech::serialization::container;

TEST_CASE( "SPEECH_SERIALIZE_EMPTY test for emtpy struct" , "[serialization]" )
{
    struct empty_structure
    {
        SPEECH_SERIALIZE_EMPTY
    };

    QDataStream ss;
    empty_structure empty;
    ss << empty;
    ss >> empty;

    QBuffer buffer;
    buffer.open( QIODevice::ReadWrite );
    QDebug out( &buffer );
    out << empty;

    SUCCEED();
}

TEST_CASE( "SPEECH_SERIALIZE(...)" , "[serialization]" )
{
    struct foo
    {
        QString m1;
        int m2;

        SPEECH_SERIALIZE( m1 , m2 )
    };

    QBuffer buffer;
    buffer.open( QIODevice::ReadWrite );
    QDataStream ss { &buffer };

    foo f;

    f.m1 = "test-data";
    f.m2 = qrand();

    foo another;

    // Datastream read/write
    ss << f;
    buffer.seek( 0 );
    ss >> another;

    REQUIRE ( f.m1 == another.m1 );
    REQUIRE ( f.m2 == another.m2 );

    buffer.open( QIODevice::ReadWrite );
    QDebug out( &buffer );
    out << f;

    SUCCEED();
}

TEST_CASE( "container serialization" , "[serialization]" )
{

    struct foo
    {
        std::vector<int> x;

        SPEECH_SERIALIZE( x )
    };

    QByteArray data;
    QBuffer buffer { &data };
    buffer.open( QIODevice::ReadWrite );
    QDataStream ss { &buffer };
    foo f;

    f.x.push_back( 1 );
    f.x.push_back( 2 );
    f.x.push_back( 3 );

    foo another;

    ss << f;
    buffer.seek( 0 );
    ss >> another;

    REQUIRE ( another.x.size() == 3 );
    REQUIRE ( another.x.at(0) == 1 );
    REQUIRE ( another.x.at(1) == 2 );
    REQUIRE ( another.x.at(2) == 3 );

    QDebug out( &buffer );
    out << f;

    SUCCEED();
}
