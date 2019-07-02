#include <functional>

namespace speech
{
namespace udp
{

namespace impl
{
template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue, T...>::udp_receiver_impl ( port p, QUdpSocket::BindFlag flag )
     : m_socket{ new shared_socket<QUdpSocket> ( std::make_unique<QUdpSocket>() ) }
{
     if ( !m_socket->socket().bind ( p.get(), flag ) ) {
          throw std::runtime_error ( "Socket could not bind" );
     }

     m_socket->attach ( std::bind ( &udp_receiver_impl<EnableQueue, T...>::on_data_received, this, std::placeholders::_1 ) );
}

template <bool EnableQueue, typename... T>
udp_receiver_impl<EnableQueue, T...>::udp_receiver_impl ( shared_socket<QUdpSocket>& socket )
{
     socket.attach ( std::bind ( &udp_receiver_impl<EnableQueue, T...>::on_data_received, this, std::placeholders::_1 ) );
}

template <bool EnableQueue, typename... T>
int udp_receiver_impl<EnableQueue, T...>::on_data_received ( const QByteArray& buffer )
{
     return this->parse ( buffer );
}
}
} // namespace udp
} // namespace speech
