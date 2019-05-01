#include "udp_receiver.h"

namespace speech
{
namespace udp
{
template <typename... T>
udp_receiver<T...>::udp_receiver(port p)
    : udp_receiver<T...>{m_built_in_socket, p}
{
}

template <typename... T>
udp_receiver<T...>::udp_receiver(QUdpSocket &socket)
    : m_socket(socket)
{
}

template <typename... T>
udp_receiver<T...>::udp_receiver(QUdpSocket &socket, port p)
    : m_socket{socket},
      m_port{p.get()}
{
    if (!m_socket.bind(m_port))
    {
        throw std::runtime_error("Socket could not bind");
    }

    QObject::connect(&m_socket, &QUdpSocket::readyRead, [this]() {
        on_data_received();
    });
}

template <typename... T>
void udp_receiver<T...>::on_data_received()
{
    while (m_socket.hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(m_socket.pendingDatagramSize());

        //Read datagram
        QHostAddress client_address;
        quint16 client_port;
        m_socket.readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);

        m_buffer.append(datagram);

        auto number_of_bytes_processed = this->parse(m_buffer);

        //Clear processed bytes from buffer
        m_buffer.remove(0, static_cast<int>(number_of_bytes_processed));
    }
}
} // namespace udp
} // namespace speech