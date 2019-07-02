#ifndef SPEECH_UDP_RECEIVER_H
#define SPEECH_UDP_RECEIVER_H

#include "speech/receiver.h"
#include "speech/util.h"
#include <QUdpSocket>
#include <memory>
#include "speech/shared_socket.h"

namespace speech
{
namespace udp
{

namespace impl
{

template <bool EnableQueue, typename... T>
class udp_receiver_impl : public receiver<EnableQueue, T...>
{
public:

     explicit udp_receiver_impl ( port, QUdpSocket::BindFlag = QUdpSocket::DefaultForPlatform );
     explicit udp_receiver_impl ( shared_socket<QUdpSocket>& );

private:

     int on_data_received ( const QByteArray& );
     std::unique_ptr<shared_socket<QUdpSocket>> m_socket;

};

}

template<typename... T>
using  udp_receiver = impl::udp_receiver_impl<false,  T...>;

template<typename... T>
using queued_udp_receiver = impl::udp_receiver_impl<true, T...>;

} // namespace udp
} // namespace speech

#include "udp_receiver_impl.h"

#endif
