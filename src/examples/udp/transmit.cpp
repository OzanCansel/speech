#include <QDataStream>
#include <QCoreApplication>
#include <QHostAddress>
#include <QThread>
#include <speech/udp/udp_transmitter.h>
#include <memory>
#include "models.h"

int main(int argc, char **argv)
{
    using namespace speech::udp;
    using namespace std;

    QCoreApplication app(argc, argv);

    auto shared_sck = make_shared<QUdpSocket>();
    auto unique_sck = make_unique<QUdpSocket>();
    udp_transmitter<greeting, roll_dice> udp{QHostAddress{QHostAddress::LocalHost}, speech::port(12345)};
    udp_transmitter<greeting, roll_dice> udp_shared{ shared_sck , QHostAddress::LocalHost, speech::port(12345) };
    udp_transmitter<greeting, roll_dice> udp_unique{ std::move(unique_sck) , QHostAddress::LocalHost, speech::port(12345)};

    for (int i = 0;; ++i)
    {
        greeting greeting_msg;
        greeting_msg.my_name_is = QString("%0 - %1").arg("Hi ! My name is Ozan -").arg(i);
        udp.transmit(greeting_msg);
        udp_shared.transmit(greeting_msg);
        udp_unique.transmit(greeting_msg);
        
        qDebug() << "transmit => " << greeting_msg;
        QCoreApplication::processEvents();
        QThread::msleep(rand() % 1000);

        roll_dice dice;
        udp.transmit(dice);
        udp_shared.transmit(dice);
        udp_unique.transmit(dice);

        qDebug() << "transmit => " << dice;
        QCoreApplication::processEvents();
        QThread::msleep(rand() % 1000);
    }

    return app.exec();
}