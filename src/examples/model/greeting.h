#ifndef SPEECH_GREETING_H
#define SPEECH_GREETING_H

#include <QDataStream>
#include <QDebug>

struct greeting {
     QString my_name_is;
};

QDataStream& operator>> ( QDataStream& in, greeting& greeting )
{
     return in >> greeting.my_name_is;
}

QDataStream& operator<< ( QDataStream& out, const greeting& greeting )
{
     return out << greeting.my_name_is;
}


QDebug operator<< ( QDebug out, const greeting& greeting )
{
     return out << "{ " << greeting.my_name_is << " }";
}

#endif
