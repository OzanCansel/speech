#ifndef SPEECH_MODELS_H
#define SPEECH_MODELS_H

#include <QDataStream>
#include <QDebug>
#include <QDateTime>

struct greeting
{
    QString my_name_is;
};

struct roll_dice
{
    int chance { rand() % 100 };
    QDateTime timestamp { QDateTime::currentDateTime() };
};

QDataStream& operator<<(QDataStream& out, const greeting& greeting)
{
    return out << greeting.my_name_is;
}

QDataStream& operator>>(QDataStream& in, greeting& greeting)
{
    return in >> greeting.my_name_is;
}

QDebug operator<<(QDebug out, const greeting& greeting)
{
    return out << "{ " << greeting.my_name_is << " }";
}

QDataStream& operator<<(QDataStream& out, const roll_dice& dice)
{
    return out << dice.chance << dice.timestamp;
}

QDataStream& operator>>(QDataStream& in, roll_dice& dice)
{
    return in >> dice.chance >> dice.timestamp;
}

QDebug operator<<(QDebug out, const roll_dice& dice)
{
    out << "{ chance : " << dice.chance << " , timestamp : " << dice.timestamp.toString(Qt::DateFormat::ISODate);
    return out;
}

#endif