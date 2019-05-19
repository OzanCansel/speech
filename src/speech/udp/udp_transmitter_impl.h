#include "speech/handle/unique_ptr_handle.h"
#include "speech/handle/shared_ptr_handle.h"

namespace speech
{
namespace udp
{
template <typename... T>
udp_transmitter<T...>::udp_transmitter(const QHostAddress& addr, const speech::port& p)
    : m_addr{ addr },
      m_port{ p.get() } ,
      m_socket{ new speech::handle::unique_ptr_handle<QUdpSocket>{ std::make_unique<QUdpSocket>() } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter(QUdpSocket& socket, const QHostAddress& addr, const speech::port& p)
    :
    m_addr{ addr }  ,
    m_port{ p.get() } ,
    m_socket{  new speech::handle::handle<QUdpSocket>{ socket } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter(std::unique_ptr<QUdpSocket>  socket, const QHostAddress& addr, const speech::port& p)
    :
    m_addr{  addr } ,
    m_port{  p.get() } ,
    m_socket{  new speech::handle::unique_ptr_handle<QUdpSocket>{  std::move(socket) } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter(std::shared_ptr<QUdpSocket> socket, const QHostAddress& addr, const speech::port& p)
    :
    m_addr{ addr }  ,
    m_port{ p.get() } ,
    m_socket{ new speech::handle::shared_ptr_handle<QUdpSocket>{ socket } }
{   }

template <typename... T>
int udp_transmitter<T...>::port() const
{
    return m_port;
}

template <typename... T>
QHostAddress udp_transmitter<T...>::destination() const
{
    return m_addr;
}

template <typename... T>
bool udp_transmitter<T...>::write(const QByteArray &data)
{
    auto& socket = m_socket->ref();

    socket.writeDatagram(data, m_addr, m_port);
    socket.waitForBytesWritten();

    return true;
}
} // namespace udp
} // namespace speech
