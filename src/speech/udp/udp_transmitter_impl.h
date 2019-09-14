#include "speech/handle/unique_ptr_handle.h"
#include "speech/handle/shared_ptr_handle.h"
#include "udp_transmitter.h"

namespace speech
{
namespace udp
{
template <typename... T>
udp_transmitter<T...>::udp_transmitter ( const QHostAddress& addr, const speech::port& p )
     : m_addr{ addr },
       m_port{ p.get() },
       m_socket{ new speech::handle::unique_ptr_handle{ std::make_unique<QUdpSocket>() } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter ( QUdpSocket& socket, const QHostAddress& addr, const speech::port& p )
     :
     m_addr{ addr },
     m_port{ p.get() },
     m_socket{  new speech::handle::handle<QUdpSocket>{ socket } }
{   }

template<typename... T>
template<typename Deleter>
udp_transmitter<T...>::udp_transmitter ( std::unique_ptr<QUdpSocket , Deleter>  socket, const QHostAddress& addr, const speech::port& p )
     :
     m_addr{  addr },
     m_port{  p.get() },
     m_socket{  new speech::handle::unique_ptr_handle{  std::move ( socket ) } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter ( std::shared_ptr<QUdpSocket> socket, const QHostAddress& addr, const speech::port& p )
     :
     m_addr{ addr },
     m_port{ p.get() },
     m_socket{ new speech::handle::shared_ptr_handle<QUdpSocket>{ socket } }
{   }

template<typename... T>
udp_transmitter<T...>::~udp_transmitter() noexcept
{
    m_socket->ref().waitForBytesWritten();
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
bool udp_transmitter<T...>::write ( const QByteArray &data )
{
     auto& socket = m_socket->ref();

     //Split into 512 bytes
     //Here is why 512 bytes : https://doc.qt.io/qt-5/qudpsocket.html#writeDatagram
     if ( data.length() < 512 ) {
          auto res = socket.writeDatagram ( data, m_addr, m_port );
          return res == data.length();
     } else {
          for ( auto i = 0; i < data.length(); i += 512 ) {
               auto curr_data = data.mid ( i, 512 );
               auto res = socket.writeDatagram ( curr_data, m_addr, m_port );

               if ( res != curr_data.length() ) {
                    return false;
               }

          }
     }

     return false;
}

template<typename T, typename Socket>
void transmit ( const T& entity, const QHostAddress& host, const speech::port& p, Socket socket )
{


     static_assert ( std::is_same<Socket, std::reference_wrapper<QUdpSocket>>::value ||
                     std::is_same<Socket, std::shared_ptr<QUdpSocket>>::value,
                     "Socket must be one of these type of sockets : [ QUdpSocket, std::shared_ptr<QUdpSocket> ]" );


     udp_transmitter<T> udp{ socket, host, p };

     udp.transmit ( entity );
}

template<typename T>
void transmit ( const T& entity, const QHostAddress& host, const speech::port& p )
{
     QUdpSocket socket;
     transmit ( entity, host, p, std::ref ( socket ) );
}

} // namespace udp
} // namespace speech
