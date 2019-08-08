namespace speech
{
namespace udp
{

namespace impl
{
template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>::udp_receiver_impl(port p , QAbstractSocket::BindMode mode)
    :   m_socket{ new speech::handle::unique_ptr_handle<QUdpSocket>{  std::make_unique<QUdpSocket>() } }
{
    if (!m_socket->ref().bind(p.get() , mode))
    {
        throw std::runtime_error("Socket could not bind");
    }

    m_signal_slot_conn = QObject::connect(&m_socket->ref(), &QUdpSocket::readyRead, [this]() {
        on_data_received();
    });
}

template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>::udp_receiver_impl(QUdpSocket &socket)
    : m_socket{ new speech::handle::handle<QUdpSocket>{ socket } }
{}

template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue ,T...>::udp_receiver_impl(QUdpSocket &socket, port p , QAbstractSocket::BindMode mode)
    : m_socket{ new speech::handle::handle<QUdpSocket>{ socket } }
{
    if (!m_socket->ref().bind(p.get() , mode))
    {
        throw std::runtime_error("Socket could not bind");
    }

    m_signal_slot_conn = QObject::connect(&m_socket->ref(), &QUdpSocket::readyRead, [this]() {
        on_data_received();
    });
}

template<bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>::udp_receiver_impl(udp_receiver_impl<EnableQueue , T...>&& rhs)
{
    m_buffer = std::move(rhs.m_buffer);
    m_socket = std::move(rhs.m_socket);

    //Disconnect other object's signal
    QObject::disconnect(rhs.m_signal_slot_conn);
    m_signal_slot_conn = QObject::connect(&m_socket->ref(), &QUdpSocket::readyRead, [this]() {
        on_data_received();
    });
}

template<bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue , T...>& udp_receiver_impl<EnableQueue , T...>::operator=(udp_receiver_impl<EnableQueue , T...>&& rhs)
{
    m_buffer = std::move(rhs);
    m_socket = std::move(rhs.m_socket);

    //Disconnect other object's signal
    QObject::disconnect(rhs.m_signal_slot_conn);
    m_signal_slot_conn = QObject::connect(&m_socket->ref(), &QUdpSocket::readyRead, [this]() {
        on_data_received();
    });
}

template <bool EnableQueue, typename... T>
void udp_receiver_impl<EnableQueue , T...>::on_data_received()
{
    auto& socket = m_socket->ref();
    while (socket.hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(socket.pendingDatagramSize());

        //Read datagram
        QHostAddress client_address;
        quint16 client_port;
        socket.readDatagram(datagram.data(), datagram.size(), &client_address, &client_port);

        m_buffer.append(datagram);

        auto number_of_bytes_processed = this->parse(m_buffer);

        //Clear processed bytes from buffer
        m_buffer.remove(0, static_cast<int>(number_of_bytes_processed));
    }
}
}
} // namespace udp
} // namespace speech
