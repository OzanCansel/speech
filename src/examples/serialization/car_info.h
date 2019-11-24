#ifndef SPEECH_CAR_INFO_H
#define SPEECH_CAR_INFO_H

#include <QObject>
#include <speech/serialize/qobject_serialize.h>

using namespace speech::serialization::qobject;

class car_info : public QObject
{
     Q_OBJECT
     Q_PROPERTY ( QString name READ name WRITE set_name )

public:

     QString name();
     void set_name( const QString& );

private:

     QString m_name;

};

#endif
