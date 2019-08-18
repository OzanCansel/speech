#ifndef SPEECH_GREETING_H
#define SPEECH_GREETING_H

#include <QDataStream>
#include <QDebug>
#include <vector>
#include <speech/serialize/container_serialize.h>

using namespace speech::serialization::container;

struct greeting
{
    QString my_name_is;
    std::vector<int> numbers;
};

QDataStream& operator>>(QDataStream& in, greeting& greeting)
{
    return in >> greeting.my_name_is >> greeting.numbers;
}

QDataStream& operator<<(QDataStream& out, const greeting& greeting)
{
    return out << greeting.my_name_is << greeting.numbers;
}

QDebug operator<<(QDebug out, const greeting& greeting)
{
    return out << "{ " << greeting.my_name_is << " numbers : " << greeting.numbers << " }";
}

#endif
