#ifndef SPEECH_TCP_RECEIVER_H
#define SPEECH_TCP_RECEIVER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include "speech/receiver.h"
#include "speech/util.h"
#include "speech/shared_socket.h"

namespace speech
{
namespace tcp
{
namespace impl
{

template <bool EnableQueue, typename... T>
class tcp_receiver_impl : protected receiver<EnableQueue, T...>
{
public:

     using socket_type = QTcpSocket;
     int on_data_received ( const QByteArray & , std::weak_ptr<QTcpSocket>&& );

     std::weak_ptr<QTcpSocket> socket() { return m_sck; }

protected:

     inline QTcpSocket &device();

private:

     std::weak_ptr<QTcpSocket> m_sck {  };

};
} // namespace impl

template<typename... T>
using tcp_receiver = impl::tcp_receiver_impl<false, T...>;

template<typename... T>
using queued_tcp_receiver = impl::tcp_receiver_impl<true, T...>;

} // namespace tcp
} // namespace speech

#include "tcp_receiver_impl.h"

#endif
