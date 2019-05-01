#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <speech/udp/udp_receiver.h>
#include "models.h"


int main(int argc, char **argv)
{

    using namespace speech::udp;

    QCoreApplication app(argc, argv);

    udp_receiver<greeting, roll_dice> receiver{ speech::port(12345) };

    QTimer checkMessages;

    checkMessages.setInterval(30);
    QObject::connect(&checkMessages, &QTimer::timeout, [&]() {
        while (!receiver.messages<roll_dice>().empty())
        {
            qDebug() << "roll_dice => " << receiver.messages<roll_dice>().front();
            receiver.messages<roll_dice>().pop();
        }

        while (!receiver.messages<greeting>().empty())
        {
            qDebug() << "greeting => " << receiver.messages<greeting>().front();
            receiver.messages<greeting>().pop();
        }

    });

    checkMessages.start();

    return app.exec();
}