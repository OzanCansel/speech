#ifndef SPEECH_QUESTIONS_H
#define SPEECH_QUESTIONS_H

#include <QDataStream>
#include <QString>
#include <speech/chit_chat/fellow.h>

struct what_is_your_name{};
struct ok_then_go_to_home{};
struct ok_never_mind{};
struct how_many_logs_do_you_have{};

template<>
struct speech::chit_chat::answer<what_is_your_name>
{
    QString name;
};

QDataStream& operator<<(QDataStream& out, const speech::chit_chat::answer<what_is_your_name>& answer)
{
    return out << answer.name;
};

QDataStream& operator>>(QDataStream& in, speech::chit_chat::answer<what_is_your_name>& answer)
{
    return in >> answer.name;
}

#endif