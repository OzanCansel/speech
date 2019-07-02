#ifndef SPEECH_ENTITY_H
#define SPEECH_ENTITY_H

#include <QObject>
#include <QString>

class entity : public QObject
{
     Q_OBJECT
     Q_PROPERTY ( QString name READ name WRITE set_name )
     Q_PROPERTY ( int identity READ identity WRITE set_identity )

public:
     QString name();
     void set_name ( QString );
     int identity();
     void set_identity ( int );

private:

     QString m_name;
     int m_identity{ };
};

#endif
