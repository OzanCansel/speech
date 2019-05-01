#include <QCoreApplication>
#include <QThread>
#include <speech/udp/udp_transmitter.h>
#include <speech/qobject_serialization.h>
#include "entity.h"

// template QDataStream& operator<<(QDataStream& , const entity&);
// template QDataStream& operator>>(QDataStream& , entity&);

int main(int argc, char **argv)
{
    using namespace speech::udp;
    using namespace speech;

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("speech-udp-qobject-serialization-transmitter");

    udp_transmitter<entity> transmitter{QHostAddress{QHostAddress::LocalHost}, speech::port(12345)};
    for (int i = 0;; ++i)
    {
        entity e;
        e.set_name(QString("Ozan - %0").arg(i));
        e.set_identity(i * rand() % 100);
        transmitter.transmit(e);
        QCoreApplication::processEvents();

        QThread::msleep(1000);
    }

    return app.exec();
}