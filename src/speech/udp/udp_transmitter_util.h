#ifndef SPEECH_UDP_TRANSMITTER_UTIL_H
#define SPEECH_UDP_TRANSMITTER_UTIL_H

#include "udp_transmitter.h"
#include <utility>

namespace speech
{
namespace udp
{

template<typename T, typename Socket>
void transmit ( const T&, const QHostAddress&, const speech::port&, Socket&& );

template<typename T>
void transmit ( const T&, const QHostAddress&, const speech::port& );

template<typename T>
void broadcast ( const T& , const speech::port& );

template<typename T>
void broadcast ( const T& , const speech::port& , const QNetworkInterface& );

template<typename T>
void broadcast ( const T& , const speech::port& , const QList<QNetworkInterface>& );


} // namespace udp
} // namespace speech

#include "udp_transmitter_util_impl.h"

#endif
