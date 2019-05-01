#include "udp_transmitter.h"
#include <QUdpSocket>

namespace speech
{
namespace udp
{
template <typename... T>
udp_transmitter<T...>::udp_transmitter(QHostAddress addr, speech::port p)
    : m_addr{addr},
      m_port{p.get()}
{
}

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
    QUdpSocket socket;

    socket.writeDatagram(data, m_addr, m_port);
    socket.waitForBytesWritten();

    return true;
}
} // namespace udp
} // namespace speech
