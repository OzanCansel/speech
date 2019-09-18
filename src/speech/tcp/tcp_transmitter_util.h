#ifndef SPEECH_TCP_TRANSMITTER_UTIL_H
#define SPEECH_TCP_TRANSMITTER_UTIL_H

#include <QHostAddress>
#include "speech/util.h"

namespace speech
{
namespace tcp
{

template<typename T, typename Socket>
void transmit( const T& , const QHostAddress&, const speech::port& , Socket&& );

template<typename T, typename Socket>
void transmit( const T&, Socket&& );

template<typename T>
void transmit( const T& , const QHostAddress& , const speech::port& );

}
}

#include "tcp_transmitter_util_impl.h"

#endif
