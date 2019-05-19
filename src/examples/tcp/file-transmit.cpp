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
        { 
            {{"a", "addr"} , "Specify host ip" , "address" } ,
            {{"p", "port"} , "Specify host port number" , "port number" } , 
            {{"f", "file"} , "Specify a file which will be transmitted" , "file path"   }
        });

    parser.process(app);

    //Defaults
    auto port = 24942;
    QString file;
    QString host;

    if (parser.isSet("p"))
        port = parser.value("p").toInt();

    if (parser.isSet("a"))
        host = parser.value("a");

    if(parser.isSet("f"))
        file = parser.value("f");
    else
        throw "File did not specified, (--file some-file.txt)";

    QTcpSocket sck;
    shared_socket<QTcpSocket> shared{ sck };
    file_transmitter t { shared, QHostAddress(host) , speech::port(port) };

    t.send(file);

    return app.exec();
}