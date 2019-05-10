#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <speech/tcp/tcp_server.h>
#include <speech/receiver.h>
#include "greeting.h"
#include "roll_dice.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;
    using namespace speech::impl;

    auto server = make_server( QHostAddress(QHostAddress::LocalHost) ,  speech::port(24942) ,
        make_handler_2<greeting>([](const greeting& greeting){
            qDebug() << greeting;
        }) ,
        make_handler_2<roll_dice>([](const roll_dice& dice){
            qDebug() << dice;
        })
    );

    app.exec();
}