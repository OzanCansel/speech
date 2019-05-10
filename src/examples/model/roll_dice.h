#ifndef SPEECH_ROLL_DICE_H
#define SPEECH_ROLL_DICE_H

#include <QDateTime>

struct roll_dice
{
    int chance { rand() % 100 };
    QDateTime timestamp { QDateTime::currentDateTime() };
};

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