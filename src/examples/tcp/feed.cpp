#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QCommandLineParser>
#include <speech/tcp/tcp_transmitter.h>
#include <greeting.h>
#include <roll_dice.h>

int main(int argc , char** argv)
{
    using namespace speech;
    using namespace speech::tcp;

    QCoreApplication app(argc , argv);


    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions(
        { {{"p", "port"} , "Specify host port number" , "port number" } , 
          {{"f", "fps"} , "Specify transmitting rate" , "frame per second"   }
        });

    parser.process(app);

    //Defaults
    auto port = 24942;
    auto fps = 2;

    if (parser.isSet("p"))
        port = parser.value("p").toInt();

    if(parser.isSet("f"))
        fps = parser.value("f").toInt();

    auto delay = 1000.0 / fps / 2.0;

    tcp_transmitter<greeting , roll_dice> tcp{ QHostAddress(QHostAddress::LocalHost) , speech::port(port) };

    for(auto i = 0;;++i)
    {
        greeting g;

        g.my_name_is = QString("Hello %0").arg(i);
        tcp.transmit(g);
        qDebug() << "transmit => " << g;

        QThread::msleep(delay);
        QCoreApplication::processEvents();

        roll_dice dice;

        dice.chance = rand() % 100;\
        tcp.transmit(dice);

        qDebug() << "transmit => " << dice;
        QCoreApplication::processEvents();
        QThread::msleep(delay);
    }

    return app.exec();
}