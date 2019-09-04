#ifndef SPEECH_UDP_RECEIVER_H
#define SPEECH_UDP_RECEIVER_H

#include "speech/receiver.h"
#include "speech/util.h"
#include "speech/handle/unique_ptr_handle.h"
#include "speech/handle/shared_ptr_handle.h"
#include <memory>
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

    udp_receiver_impl(port , QAbstractSocket::BindMode = QAbstractSocket::DefaultForPlatform);
    udp_receiver_impl(QUdpSocket &);
    udp_receiver_impl(QUdpSocket &, port , QAbstractSocket::BindMode = QAbstractSocket::DefaultForPlatform);
    udp_receiver_impl<EnableQueue, T...>& operator=(const udp_receiver_impl<EnableQueue , T...>&) = delete;
    udp_receiver_impl(udp_receiver_impl&&);
    udp_receiver_impl<EnableQueue, T...>& operator=(udp_receiver_impl<EnableQueue , T...>&&);
    QHostAddress incoming_addr() const { return m_client_address; };
    quint16 incoming_port() const { return m_client_port; };

private:

    void on_data_received();

    QByteArray m_buffer;
    //Read datagram
    QHostAddress m_client_address;
    quint16 m_client_port;
    std::unique_ptr<speech::handle::handle<QUdpSocket>>  m_socket;
    QMetaObject::Connection m_signal_slot_conn;
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
