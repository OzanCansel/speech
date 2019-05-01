#include <QDataStream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include <speech/udp/udp_transmitter.h>
#include "models.h"

int main(int argc, char **argv)
{
    using namespace speech::udp;

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("speech-udp-transmitter");

    udp_transmitter<greeting, roll_dice> transmitter{QHostAddress{QHostAddress::LocalHost}, speech::port(12345)};
    for (int i = 0;; ++i)
    {
        greeting greeting_msg;
        greeting_msg.my_name_is = QString("%0 - %1").arg("Hi ! My name is Ozan -").arg(i);
        transmitter.transmit(greeting_msg);
        qDebug() << "transmit => " << greeting_msg;
        QCoreApplication::processEvents();
        QThread::msleep(rand() % 1000);
        roll_dice dice;
        transmitter.transmit(dice);
        qDebug() << "transmit => " << dice;
        QCoreApplication::processEvents();
        QThread::msleep(rand() % 1000);
    }

    return app.exec();
}