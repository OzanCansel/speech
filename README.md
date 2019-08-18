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
- A compiler which supports C++14 and higher

# Compiler Support
- GCC
- MinGW
- VS Compiler
- Clang

## Define Models
```c++
//models.h
#ifndef SPEECH_MODELS_H
#define SPEECH_MODELS_H

#include <QDataStream>
#include <QDebug>
#include <QDateTime>

struct greeting
{
    QString my_name_is;
};

struct roll_dice
{
    int chance { rand() % 100 };
    QDateTime timestamp { QDateTime::currentDateTime() };
};

//State how to serialize/deserialize once
QDataStream& operator>>(QDataStream& in, greeting& greeting)
{
    return in >> greeting.my_name_is;
}

QDataStream& operator<<(QDataStream& out, const greeting& greeting)
{
    return out << greeting.my_name_is;
}

QDebug operator<<(QDebug out, const greeting& greeting)
{
    return out << "{ " << greeting.my_name_is << " }";
}

QDataStream& operator<<(QDataStream& out, const roll_dice& dice)
{
    return out << dice.chance << dice.timestamp;
}

QDataStream& operator>>(QDataStream& in, roll_dice& dice)
{
    return in >> dice.chance >> dice.timestamp;
}

QDebug operator<<(QDebug out, const roll_dice& dice)
{
    return out << "{ chance : " << dice.chance << " , timestamp : " << dice.timestamp.toString(Qt::DateFormat::ISODate) << " }";
}

#endif
```

## tcp server
```c++
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include "models.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;

    auto server = make_server( QHostAddress::Any ,  speech::port(24942) ,
        make_handler_f<greeting>([](const greeting& greeting , QTcpSocket&){
            qDebug() << greeting;
        }) 
        ,
        make_handler_f<roll_dice>([](const roll_dice& dice , QTcpSocket&){
            qDebug() << dice;
        })
    );

    app.exec();
}
```

## tcp transmit
```c++
//a main .cpp file
#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <speech/tcp/tcp_transmitter.h>
#include "models.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;

    tcp_transmitter<greeting , roll_dice> tcp{ QHostAddress::LocalHost , speech::port(24942) };
    tcp.transmit(greeting{ "my name is speech-lib" });
    tcp.transmit(roll_dice{});
    app.exec();
}
```

## udp transmit
```c++
//a main .cpp file
#include <QCoreApplication>
#include <speech/udp/udp_transmitter.h>
#include "models.h"

//Now, we can transmit our models on desired protocol
int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    using namespace speech;
    using namespace speech::udp;

    udp_transmitter<greeting, roll_dice> udp{QHostAddress{QHostAddress::Broadcast}, speech::port(12345)};

    //Those models will be broadcasted
    udp.transmit(greeting{ "my name is speech-lib" });
    udp.transmit(roll_dice{ });

    app.exec();
}
```
## udp receive with inheritance
```c++
//a main .cpp file
#include <QCoreApplication>
#include <QDebug>
#include "models.h"
#include <speech/udp/udp_receiver.h>

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
#include <QTimer>
#include <speech/udp/udp_receiver.h>
#include "models.h"

int main(int argc, char **argv)
{
    using namespace speech::udp;

    QCoreApplication app(argc, argv);

    queued_udp_receiver<greeting, roll_dice> udp{ speech::port(12345) };

    QTimer checkMessages;

    checkMessages.setInterval(30);
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
