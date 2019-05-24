#include <QCoreApplication>
#include <QHostAddress>
#include <speech/chit_chat/fellow.h>
#include "questions.h"

int main(int argc, char** argv)
{

    using namespace speech;
    using namespace speech::chit_chat;

    QCoreApplication app(argc , argv);

    fellow foreigner{ QHostAddress::LocalHost , speech::port(24942) };
    // fellow pvtks{ QHostAddress::LocalHost , speech::port(24942) };

    auto res = (foreigner, what_is_your_name{});



    return app.exec();
}