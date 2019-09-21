#include "speech/error/connection_error.h"
#include "tcp_receiver.h"
#include <functional>

namespace speech
{
namespace tcp
{

namespace impl
{

template <bool EnableQueue, typename... T>
int tcp_receiver_impl<EnableQueue, T...>::on_data_received ( const QByteArray &buffer , std::weak_ptr<QTcpSocket>&& sck )
{
    m_sck = std::forward<std::weak_ptr<QTcpSocket>>( sck );
    return this->parse ( buffer );
}
} // namespace impl
} // namespace tcp
} // namespace speech
