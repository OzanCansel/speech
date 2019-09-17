## develop [![Build status](https://ci.appveyor.com/api/projects/status/p2ddxq76pqmboj74/branch/develop?svg=true)](https://ci.appveyor.com/project/OzanCansel/speech/branch/develop)
## master [![Build status](https://ci.appveyor.com/api/projects/status/p2ddxq76pqmboj74/branch/master?svg=true)](https://ci.appveyor.com/project/OzanCansel/speech/branch/master)


# speech

This project aims to develop a qt-based generic serialization/deserialization mechanism and strongly-typed messaging over network.

The library is composed of only headers. We aim to follow modern c++ approach.

It is under GNU General Public License.

# CMake
speech library uses CMake build system. We are using modern-cmake approach and paradigm which enforces us to use CMake 3.1x.

# QMake
speech library doesn't use qmake as build system currently but it will be supported at future.

# Dependencies and required(recommended) tools
- Qt5
- CMake
- A compiler which supports C++17

# Compiler Support
- GCC
- MinGW
- VS Compiler
- Clang

# Compile
## Windows 
### Qt - MinGW
Run git bash shell as administrator
```
git clone https://github.com/OzanCansel/speech
cd speech
mkdir build
cd build
## example run => cmake -G"MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=/c/Qt/Tools/mingw730_64/bin/mingw32-make.exe -DCMAKE_PREFIX_PATH=/c/Qt/5.12.2/mingw73_64/lib/cmake ..
cmake -G"MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=<make_executable_path> -DCMAKE_PREFIX_PATH=<qt_cmake_definitions_path> ..
cmake --build . --target install -- -j$(nproc)
```

## Example CMakeLists.txt Usage
```
project(speech-usage)
cmake_minimum_required(VERSION 3.1)

find_package( speech REQUIRED )

add_executable( speech-usage your_main.cpp )

target_link_libraries( speech-usage speech::speech )
```

## tcp server
```c++
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <speech/speech.h>

struct greeting
{
    QString my_name_is;

    SPEECH_SERIALIZE( my_name_is )
};

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };

     SPEECH_SERIALIZE( chance , timestamp )
};

int main ( int argc, char** argv )
{
    QCoreApplication app ( argc, argv );

    using namespace speech;
    using namespace speech::tcp;

    auto port = 24942;

    tcp_server server { QHostAddress::Any , speech::port { port } };

    auto listeners =
            listen<roll_dice>( []( const roll_dice& e , QTcpSocket& ) {
        qDebug() << "Received : " << e;
    }) |
            listen<greeting>( [] ( const greeting& g , QTcpSocket& ){
        qDebug() << "Received : " << g;
    });

    server.listen( listeners );

    qDebug() << "Tcp Server running at" << server.port() << "port";

    return QCoreApplication::exec();
}
```

## tcp transmit
```c++
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <speech/speech.h>

struct greeting
{
    QString my_name_is;

    SPEECH_SERIALIZE( my_name_is )
};

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };

     SPEECH_SERIALIZE( chance , timestamp )
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;

    tcp_transmitter<greeting , roll_dice> tcp{ QHostAddress::LocalHost , speech::port { 24942 } };
    tcp.transmit( greeting { "from speech-lib" } );
    tcp.transmit( roll_dice {} );

    // Transmit without instance
    transmit( roll_dice {} , QHostAddress::LocalHost , speech::port { 24942 } );

    app.exec();
}
```

## udp transmit
```c++
//a main .cpp file
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <speech/speech.h>

struct greeting
{
    QString my_name_is;

    SPEECH_SERIALIZE( my_name_is )
};

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };

     SPEECH_SERIALIZE( chance , timestamp )
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    using namespace speech;
    using namespace speech::udp;

    udp_transmitter<greeting, roll_dice> udp { QHostAddress { QHostAddress::Broadcast } , speech::port { 12345 } };

    udp.transmit( greeting{ "from speech-lib" } );
    udp.transmit( roll_dice{ } );

    // Transmit without instance
    transmit( roll_dice { } , QHostAddress {  QHostAddress::Broadcast } ,  speech::port { 12345 } );

    app.exec();
}
```
## udp receive with inheritance
```c++
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <speech/speech.h>

struct greeting
{
    QString my_name_is;

    SPEECH_SERIALIZE( my_name_is )
};

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };

     SPEECH_SERIALIZE( chance , timestamp )
};

struct my_receiver : speech::udp::udp_receiver<greeting , roll_dice>
{

    my_receiver() : speech::udp::udp_receiver<greeting , roll_dice>{ speech::port(12345) } { }

    protected:

        void on_receive(const greeting& greeting) override
        {
            qDebug() << "receive => " << greeting;
        }

        void on_receive(const roll_dice& dice) override
        {
            qDebug() << "receive => " << dice;
        }
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    my_receiver receiver;

    return app.exec();
}
```

## udp receive from queue
```c++
//a main .cpp file
#include <QCoreApplication>
#include <QDebug>
#include <QDateTime>
#include <QTimer>
#include <speech/speech.h>

struct greeting
{
    QString my_name_is;

    SPEECH_SERIALIZE( my_name_is )
};

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };

     SPEECH_SERIALIZE( chance , timestamp )
};

int main(int argc, char **argv)
{
    using namespace speech::udp;

    QCoreApplication app(argc, argv);

    queued_udp_receiver<greeting, roll_dice> udp { speech::port { 12345 } };

    QTimer checkMessages;

    checkMessages.setInterval( 30 );

    QObject::connect(&checkMessages, &QTimer::timeout, [&]() {
        while (!udp.messages<roll_dice>().empty())
        {
            qDebug() << "roll_dice => " << udp.messages<roll_dice>().front();
            udp.messages<roll_dice>().pop();
        }

        while (!udp.messages<greeting>().empty())
        {
            qDebug() << "greeting => " << udp.messages<greeting>().front();
            udp.messages<greeting>().pop();
        }
    });

    checkMessages.start();

    return app.exec();
}
```
