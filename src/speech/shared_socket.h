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
     shared_socket ( Socket & );
     shared_socket ( std::shared_ptr<Socket> );
     shared_socket ( std::unique_ptr<Socket> );

     inline Socket &socket();
     void attach ( std::function<int ( const QByteArray & ) > );
     void listen();
     void on_data_received();

private:
     std::unique_ptr<speech::handle::handle<Socket>> m_socket;
     QByteArray m_buffer;
     std::vector<std::function<int ( const QByteArray & ) >> m_listeners;
};

} // namespace speech

#include "shared_socket_impl.h"

#endif
