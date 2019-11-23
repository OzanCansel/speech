#ifndef SPEECH_QUESTIONS_H
#define SPEECH_QUESTIONS_H

#include <speech/speech.h>

struct u_there
{
    SPEECH_SERIALIZE_EMPTY
};

struct roll_a_dice
{
  SPEECH_SERIALIZE_EMPTY
};

struct yes
{
    SPEECH_SERIALIZE_EMPTY
};

struct no
{
    SPEECH_SERIALIZE_EMPTY
};

template<>
struct speech::tcp::response_of< roll_a_dice >
{
    int luck {};

    SPEECH_SERIALIZE( luck )
};

#endif
