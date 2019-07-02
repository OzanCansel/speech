#ifndef SPEECH_UDP_TRANSMITTER_H
#define SPEECH_UDP_TRANSMITTER_H

#include <memory>
#include <QHostAddress>
#include <QUdpSocket>
#include "speech/util.h"
#include "speech/transmitter.h"
#include "speech/handle/handle.h"

namespace speech
{
namespace udp
{
template <typename... T>
class udp_transmitter : public transmitter<T...>
{

public:

     udp_transmitter ( const QHostAddress&, const speech::port& );
     udp_transmitter ( QUdpSocket&, const QHostAddress&, const speech::port& );
     udp_transmitter ( std::unique_ptr<QUdpSocket>, const QHostAddress&, const speech::port& );
     udp_transmitter ( std::shared_ptr<QUdpSocket>, const QHostAddress&, const speech::port& );

     int port() const;
     QHostAddress destination() const;

protected:
     bool write ( const QByteArray & ) override;

private:

     QHostAddress m_addr;
     int m_port{ -1 };
     std::unique_ptr<speech::handle::handle<QUdpSocket>> m_socket;

};

template<typename T, typename Socket>
void udp_transmit ( const T&, const QHostAddress&, const speech::port&, Socket );

template<typename T>
void udp_transmit ( const T&, const QHostAddress&, const speech::port& );

} // namespace udp
} // namespace speech

#include "udp_transmitter_impl.h"

#endif // UDPTRANSMITTER_H
