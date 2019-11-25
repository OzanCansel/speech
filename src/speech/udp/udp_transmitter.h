#ifndef SPEECH_UDP_TRANSMITTER_H
#define SPEECH_UDP_TRANSMITTER_H

#include <memory>
#include <QHostAddress>
#include <QUdpSocket>
#include <QNetworkInterface>
#include "speech/util.h"
#include "speech/transmitter.h"
#include "speech/handle/handle.h"

namespace speech
{
namespace udp
{

using interface_idx = speech::impl::named_type<unsigned int, struct interface_idx_parameter>;
const interface_idx let_the_os_choose = interface_idx { 0 };

template <typename... T>
class udp_transmitter : public transmitter<T...>
{

public:

     udp_transmitter ( const QHostAddress&, const speech::port& , const interface_idx& = interface_idx { 0 } );
     udp_transmitter ( QUdpSocket&, const QHostAddress&, const speech::port& , const interface_idx& = interface_idx { 0 } );
     template<typename Deleter>
     udp_transmitter ( std::unique_ptr<QUdpSocket , Deleter>, const QHostAddress&, const speech::port& , const interface_idx& = interface_idx { 0 } );
     udp_transmitter ( std::shared_ptr<QUdpSocket>, const QHostAddress&, const speech::port& , const interface_idx& = interface_idx { 0 } );
     ~udp_transmitter() noexcept;
     udp_transmitter ( const udp_transmitter<T...>& ) = delete;
     udp_transmitter<T...>& operator = ( const udp_transmitter<T...>& ) = delete;
     udp_transmitter ( udp_transmitter<T...>&& ) noexcept = default;
     udp_transmitter& operator = ( udp_transmitter<T...>&& ) noexcept = default;

     int port() const;
     QHostAddress destination() const;

protected:
     bool write ( const QByteArray & ) override;

private:

     QHostAddress m_addr;
     int m_port { -1 };
     unsigned int m_iface_idx {};
     std::unique_ptr<speech::handle::handle<QUdpSocket>> m_socket;

};

} // namespace udp
} // namespace speech

#include "udp_transmitter_impl.h"

#endif // UDPTRANSMITTER_H
