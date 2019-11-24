#ifndef SPEECH_ROLL_DICE_H
#define SPEECH_ROLL_DICE_H

#include <QDateTime>

struct roll_dice {
     int chance { rand() % 100 };
     QDateTime timestamp { QDateTime::currentDateTime() };
};

// Custom serialization implementation example
inline QDataStream& operator<< ( QDataStream& out, const roll_dice& dice )
{
     return out << dice.chance << dice.timestamp;
}

inline QDataStream& operator>> ( QDataStream& in, roll_dice& dice )
{
     return in >> dice.chance >> dice.timestamp;
}

inline QDebug operator<< ( QDebug out, const roll_dice& dice )
{
     out << "roll_dice { chance : " << dice.chance << " , timestamp : " << dice.timestamp.toString ( Qt::DateFormat::ISODate ) << " }";
     return out;
}

#endif
