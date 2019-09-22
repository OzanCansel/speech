#ifndef SPEECH_TCP_RECEIVER_H
#define SPEECH_TCP_RECEIVER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include <utility>
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

    tcp_receiver_impl( std::shared_ptr<QTcpSocket>&& s )
        : m_socket { std::forward<std::shared_ptr<QTcpSocket>>( s ) }
    {
        using namespace std::placeholders;
        m_socket.attach( std::bind( &tcp_receiver_impl::on_data_received , this , _1 , _2 ));
    }

protected:

    inline int on_data_received ( const QByteArray &buffer , std::weak_ptr<QTcpSocket>&& sck )
    {
        return this->parse ( buffer );
    }

private:

    shared_socket<QTcpSocket> m_socket;

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
