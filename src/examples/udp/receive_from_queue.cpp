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