#include <QCoreApplication>
#include <QDebug>
#include <QTimer>
#include <speech/udp/udp_receiver.h>
#include <speech/qobject_serialization.h>
#include "entity.h"

struct qobject_receiver : speech::udp::udp_receiver<entity>
{
    
    qobject_receiver() : speech::udp::udp_receiver<entity>{ speech::port(12345) } { }

    protected:
        
        void on_receive(const entity& greeting) override
        {
            using namespace speech;
            
            qDebug() << "receive => " << greeting;
        }
};

int main(int argc, char **argv)
{

    using namespace speech;
    using namespace speech::udp;

    QCoreApplication app(argc, argv);

    qobject_receiver receiver;

    return app.exec();
}