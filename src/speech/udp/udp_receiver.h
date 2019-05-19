#ifndef SPEECH_UDP_RECEIVER_H
#define SPEECH_UDP_RECEIVER_H

#include "speech/receiver.h"
#include "speech/util.h"
#include <QUdpSocket>

namespace speech
{
namespace udp
{

namespace impl
{

template <bool EnableQueue, typename... T>
class udp_receiver_impl : public receiver<EnableQueue , T...>
{
  public:

    udp_receiver_impl(port);
    udp_receiver_impl(QUdpSocket &);
    udp_receiver_impl(QUdpSocket &, port);

  private:
    void on_data_received();

    QUdpSocket &m_socket;
    QUdpSocket m_built_in_socket;
    int m_port{};
    QByteArray m_buffer;
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