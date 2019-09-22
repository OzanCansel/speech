#ifndef GAME_OF_EXPLORERES_MESSAGES_H
#define GAME_OF_EXPLORERES_MESSAGES_H

#include <speech/speech.h>
#include <QString>
#include <QUuid>

struct let_me_join
{
    QString nick_name;

    SPEECH_SERIALIZE( nick_name )
};

struct new_message
{
    QString message;
    QString user;

    SPEECH_SERIALIZE( user , message )
};

struct new_user_joined
{
    QString user_name;

    SPEECH_SERIALIZE( user_name )
};

struct send_message
{
    QString message;

    SPEECH_SERIALIZE( message )

};

struct disconnected
{
    QString user_name;

    SPEECH_SERIALIZE( user_name )
};

struct chat_error
{
    QString message;

    SPEECH_SERIALIZE( message )
};

#endif
