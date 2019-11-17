#ifndef SPEECH_GREETING_H
#define SPEECH_GREETING_H

#include <QDataStream>
#include <QDebug>
#include <vector>
#include <speech/speech.h>

using namespace speech::serialization::container;

struct greeting
{
    QString my_name_is;
    std::vector<int> numbers;

    SPEECH_SERIALIZE( my_name_is , numbers )

};



#endif
