#ifndef SPEECH_MY_IP_H
#define SPEECH_MY_IP_H

#include <QDataStream>
#include <QString>

struct me {
     QString ip;
     QString name;
};

inline QDataStream& operator<< ( QDataStream& out, const me& m )
{
     return out << m.ip << m.name;
}

inline QDataStream& operator>> ( QDataStream& in, me& m )
{
     return in >> m.ip >> m.name;
}

inline QDebug operator<< ( QDebug out, const me& m )
{
     return out << "{ ip : " << m.ip << " , name : " << m.name << " }";
}

#endif
