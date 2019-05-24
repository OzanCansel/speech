#include "speech/error/connection_error.h"
#include <functional>

namespace speech
{
namespace tcp
{

namespace impl
{
template <bool EnableQueue, typename... T>
tcp_receiver_impl<EnableQueue, T...>::tcp_receiver_impl(shared_socket<QTcpSocket> &shared_sck)
    : m_socket{shared_sck.socket()}
{
    using namespace std::placeholders;
    shared_sck.attach(std::bind(&tcp_receiver_impl<EnableQueue, T...>::on_data_received, this, _1));
}

template <bool EnableQueue, typename... T>
QTcpSocket &tcp_receiver_impl<EnableQueue, T...>::device()
{
    return m_socket;
}

template <bool EnableQueue, typename... T>
int tcp_receiver_impl<EnableQueue, T...>::on_data_received(const QByteArray &buffer)
{
    return this->parse(buffer);
}
} // namespace impl
} // namespace tcp
} // namespace speech