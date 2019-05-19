namespace speech
{
namespace udp
{

namespace impl
{
template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>::udp_receiver_impl(port p)
    : udp_receiver_impl<EnableQueue , T...>{m_built_in_socket, p}
{
}

template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>::udp_receiver_impl(QUdpSocket &socket)
    : m_socket(socket)
{
}

template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue ,T...>::udp_receiver_impl(QUdpSocket &socket, port p)
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

template <bool EnableQueue, typename... T>
void udp_receiver_impl<EnableQueue , T...>::on_data_received()
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
}
} // namespace udp
} // namespace speech