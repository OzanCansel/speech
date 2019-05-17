#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QCommandLineParser>
#include <speech/tcp/file_transmitter.h>

int main(int argc , char** argv)
{
    using namespace speech;
    using namespace speech::tcp;

    QCoreApplication app(argc , argv);


    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions(
        { {{"p", "port"} , "Specify host port number" , "port number" } , 
          {{"f", "file"} , "Specify a file which will be transmitted" , "file path"   }
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

    QTcpSocket sck;
    shared_socket<QTcpSocket> shared{sck};
    file_transmitter t { shared, QHostAddress("10.230.1.96") , speech::port(24942) };
    // file_transmitter t { shared, QHostAddress::LocalHost , speech::port(24942) };

    t.send("/home/ozanc/dummy-data/file.txt");

    return app.exec();
}