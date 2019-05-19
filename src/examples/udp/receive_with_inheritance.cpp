#include <QCoreApplication>
#include <QDebug>
#include "models.h"
#include <speech/udp/udp_receiver.h>

struct my_receiver : speech::udp::udp_receiver<greeting , roll_dice>
{
    
    my_receiver() : speech::udp::udp_receiver<greeting , roll_dice>{ speech::port(12345) } { }

    protected:
        
        void on_receive(const greeting& greeting) override
        {
            qDebug() << "receive => " << greeting;
        }

        void on_receive(const roll_dice& dice) override
        {
            qDebug() << "receive => " << dice;
        }
};

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    my_receiver receiver;

    return app.exec();
}
