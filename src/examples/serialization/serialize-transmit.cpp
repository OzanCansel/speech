#include <QCoreApplication>
#include <QCommandLineParser>
#include <QThread>
#include <speech/udp/udp_transmitter.h>
#include <speech/serialize/qobject_serialize.h>
#include "car_info.h"
#include "rolls.h"

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
    auto port = 24942;
    auto fps = 2;

    if (parser.isSet("p"))
        port = parser.value("p").toInt();

    if (parser.isSet("f"))
        fps = parser.value("f").toInt();

    //twice in a second
    auto delay = 500.0 / fps;

    udp_transmitter<rolls , car_info> transmitter{QHostAddress{QHostAddress::LocalHost}, speech::port(port)};

    qDebug() << "Starting to transmit on port " << port << " with " << fps << " fps";

    for (int i = 0;; ++i)
    {
        rolls e;

        for( auto i = 0; i < 10; i++ )
            e.numbers.push_back(qrand() % 100);

        transmitter.transmit(e);

        qDebug() << "transmitting to" << port << " => " << e;

        QCoreApplication::processEvents();

        QThread::msleep(delay);

        car_info info;
        info.set_name(QString("mazda - %0").arg(i));
        transmitter.transmit(info);

        qDebug() << "transmitting to" << port << " => " << info;

        QCoreApplication::processEvents();

        QThread::msleep(delay);
    }

    return app.exec();
}
