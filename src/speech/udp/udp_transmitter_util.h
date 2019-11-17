#ifndef SPEECH_UDP_TRANSMITTER_UTIL_H
#define SPEECH_UDP_TRANSMITTER_UTIL_H

#include <utility>
#include <vector>
#include <QNetworkInterface>
#include <QList>
#include "speech/util.h"

namespace speech
{
namespace udp
{

namespace impl
{
inline std::vector<QHostAddress> broadcast_addresses_of( const QNetworkInterface& iface );
}

template<typename T, typename Socket>
inline void transmit ( const T&, const QHostAddress&, const speech::port&, Socket&& );

template<typename T>
inline void transmit ( const T&, const QHostAddress&, const speech::port& );

template<typename T>
inline void broadcast ( const T& , const speech::port& );

template<typename T>
inline void broadcast ( const T& , const speech::port& , const QNetworkInterface& );

template<typename T>
inline void broadcast ( const T& , const speech::port& , const QList<QNetworkInterface>& );


} // namespace udp
} // namespace speech

#include "udp_transmitter_util_impl.h"

#endif
