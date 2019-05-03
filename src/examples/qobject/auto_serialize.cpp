#include <QCoreApplication>
#include <QCommandLineParser>
#include <QThread>
#include <speech/udp/udp_transmitter.h>
#include <speech/qobject_serialization.h>
#include "entity.h"
#include "car_info.h"

int main(int argc, char **argv)
{
    using namespace speech::udp;
    using namespace speech;

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions(
        {{ {"p", "port"} , "Specify port number" , "port number" },
         { {"f", "fps"} , "Fps indicates the transmitting speed" , "fps"}
         });

    parser.process(app);

    //Defaults
    auto port = 12345;
    auto fps = 2;

    if (parser.isSet("p"))
    {
        qDebug() << parser.value("p");
        port = parser.value("p").toInt();
    }
    if (parser.isSet("f"))
        fps = parser.value("f").toInt();

    //twice in a second
    auto delay = 500.0 / fps;

    udp_transmitter<entity, car_info> transmitter{QHostAddress{QHostAddress::LocalHost}, speech::port(port)};

    qDebug() << "Starting to transmit on port " << port << " with " << fps << " fps";

    for (int i = 0;; ++i)
    {
        entity e;
        e.set_name(QString("ozan - %0").arg(i));
        e.set_identity(i * rand() % 100);
        transmitter.transmit(e);

        qDebug() << "transmit => " << e;

        QCoreApplication::processEvents();

        QThread::msleep(delay);

        car_info info;
        info.set_name(QString("chevrolet - %0").arg(i));
        transmitter.transmit(info);

        qDebug() << "transmit => " << info;

        QCoreApplication::processEvents();

        QThread::msleep(delay);
    }

    return app.exec();
}