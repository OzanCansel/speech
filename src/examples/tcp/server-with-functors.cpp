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
        make_handler_f<greeting>([](const greeting& greeting , QTcpSocket&){
            qDebug() << greeting;
        }) ,
        make_handler_f<roll_dice>([](const roll_dice& dice , QTcpSocket&){
            qDebug() << dice;
        })
    );

    app.exec();
}