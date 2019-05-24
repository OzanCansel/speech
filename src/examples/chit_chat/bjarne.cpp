#include <QCoreApplication>
#include <QHostAddress>
#include <QDebug>
#include <QCommandLineParser>
#include <speech/chit_chat/fellow.h>

using namespace speech::chit_chat;

#include <speech/tcp/tcp_server.h>
#include "questions.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc , argv);

    using namespace speech;
    using namespace speech::tcp;

    QCommandLineParser parser;
    parser.addHelpOption();

    parser.addOptions(
        {{ {"p", "port"} , "Specify listening port number" , "port number" }});

    parser.process(app);

    //Defaults
    auto port = 24942;

    if (parser.isSet("p"))
        port = parser.value("p").toInt();

    auto server = make_server( QHostAddress::Any ,  speech::port(24942) ,
        make_handler_f<what_is_your_name>([](const what_is_your_name&, QTcpSocket& socket){
            tcp_transmit(answer<what_is_your_name>{ "Bjarne" } , std::ref(socket));
        })
        ,
        make_handler_f<how_many_logs_do_you_have>([](const how_many_logs_do_you_have& , QTcpSocket&){
            // tcp_transmit(answer<how_>)
        })
    );

    qDebug() << "Tcp Server running at " << port << " port";

    app.exec();
}
