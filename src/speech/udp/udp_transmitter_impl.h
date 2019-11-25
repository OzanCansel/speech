#include "speech/handle/unique_ptr_handle.h"
#include "speech/handle/shared_ptr_handle.h"
#include "speech/traits/is_shared_ptr.h"
#include "speech/traits/is_unique_ptr.h"
#include <QNetworkAddressEntry>
#include <QNetworkDatagram>
#include "udp_transmitter.h"
#include <utility>

namespace speech
{
namespace udp
{
template <typename... T>
udp_transmitter<T...>::udp_transmitter ( const QHostAddress& addr, const speech::port& p , const interface_idx& iface_idx )
    : m_addr { addr },
      m_port { p.get() },
      m_iface_idx { iface_idx.get() } ,
      m_socket { new speech::handle::unique_ptr_handle{ std::make_unique<QUdpSocket>() } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter ( QUdpSocket& socket, const QHostAddress& addr, const speech::port& p , const interface_idx& iface_idx )
    :
      m_addr { addr },
      m_port { p.get() },
      m_iface_idx { iface_idx.get() } ,
      m_socket {  new speech::handle::handle<QUdpSocket>{ socket } }
{   }

template<typename... T>
template<typename Deleter>
udp_transmitter<T...>::udp_transmitter ( std::unique_ptr<QUdpSocket , Deleter>  socket, const QHostAddress& addr, const speech::port& p , const interface_idx& iface_idx )
    :
      m_addr {  addr } ,
      m_port {  p.get() } ,
      m_iface_idx { iface_idx.get() } ,
      m_socket {  new speech::handle::unique_ptr_handle{  std::move ( socket ) } }
{   }

template<typename... T>
udp_transmitter<T...>::udp_transmitter ( std::shared_ptr<QUdpSocket> socket, const QHostAddress& addr, const speech::port& p , const interface_idx& iface_idx )
    :
      m_addr { addr } ,
      m_port { p.get() } ,
      m_iface_idx { iface_idx.get() } ,
      m_socket { new speech::handle::shared_ptr_handle<QUdpSocket>{ std::move( socket ) } }
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
        QNetworkDatagram datagram { data , m_addr , static_cast<quint16>( m_port ) };
        datagram.setInterfaceIndex( m_iface_idx );
        auto res = socket.writeDatagram ( datagram );
        return res == data.length();
    }

    for ( auto i = 0; i < data.length(); i += 512 ) {
        auto curr_data = data.mid ( i, 512 );
        QNetworkDatagram datagram { curr_data , m_addr , static_cast<quint16>( m_port ) };
        datagram.setInterfaceIndex( m_iface_idx );
        auto res = socket.writeDatagram ( datagram );

        if ( res != curr_data.length() ) {
            return false;
        }

    }

    return false;
}

} // namespace udp
} // namespace speech
