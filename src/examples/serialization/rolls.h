#ifndef ROLLS_H
#define ROLLS_H

#include <speech/serialize/macro.h>
#include <speech/serialize/container_serialize.h>

using namespace speech::serialization::container;

struct rolls
{
    std::vector<int> numbers;

    SPEECH_SERIALIZE( numbers )
};

#endif // CONTAINER_SERIALIZATION_H
