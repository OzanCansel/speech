#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QHostAddress>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
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
        throw std::runtime_error("File did not specified, (--file some-file.txt)");

    std::vector<std::unique_ptr<file_transmitter>> file_senders;

    QFileInfo f_info{ file };
    QFileInfoList files;

    if(f_info.isDir())
    {
        QDir folder{f_info.filePath()};
        folder.setFilter(QDir::Files);
        files = folder.entryInfoList();
    }
    else
    {
        files.push_back(QFileInfo(file));
    }

    for(auto file : files)
    {
        auto sender = std::make_unique<file_transmitter>( std::make_unique<shared_socket<QTcpSocket>>(std::make_unique<QTcpSocket>()), QHostAddress(host) , speech::port(port) );
        sender->send(file.filePath());
        file_senders.push_back(std::move(sender));
    }

    return app.exec();
}
