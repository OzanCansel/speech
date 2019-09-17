#ifndef SPEECH_SHARED_SOCKET_H
#define SPEECH_SHARED_SOCKET_H

#include <QTcpSocket>
#include <QUdpSocket>
#include <functional>
#include "speech/handle/handle.h"
#include "speech/handle/shared_ptr_handle.h"
#include "speech/handle/unique_ptr_handle.h"

namespace speech
{

namespace impl
{
QByteArray read_all ( QTcpSocket& );
QByteArray read_all ( QUdpSocket& );
}

template <typename Socket>
class shared_socket
{
public:

     explicit shared_socket ( Socket & );
     explicit shared_socket ( std::shared_ptr<Socket> );
     explicit shared_socket ( std::unique_ptr<Socket> );
     shared_socket( const shared_socket<Socket>& ) = delete;
     shared_socket operator =( const shared_socket<Socket>& ) = delete;
     shared_socket( shared_socket<Socket>&& ) noexcept = default;
     shared_socket& operator =( shared_socket&& ) noexcept = default;
     ~shared_socket() noexcept = default;

     inline Socket &socket();
     void attach ( std::function<int ( const QByteArray& , Socket& ) > );
     void listen();
     void on_data_received();

private:

     std::unique_ptr<speech::handle::handle<Socket>> m_socket;
     QByteArray m_buffer;
     std::vector<std::function<int ( const QByteArray & , Socket& ) >> m_listeners;

};

} // namespace speech

#include "shared_socket_impl.h"

#endif
