#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include "speech/util.h"
#include "tcp_transmitter.h"
#include "tcp_transmitter_util.h"

namespace speech
{
namespace tcp
{

template<typename T, typename Socket>
void transmit( const T& entity , const QHostAddress& host , const speech::port& p , Socket&& s )
{

    static_assert(      std::is_same_v< Socket , QTcpSocket& > ||
                        is_shared_ptr_with_v<std::decay_t<Socket> , QTcpSocket> ||
                        is_unique_ptr_with_v<std::decay_t<Socket> , QTcpSocket> ,
                        "speech::udp::transmit(const T& , const QHostAddress&, port& , Socket&& ) => Socket parameter must be one of these type : [ QTcpSocket& , std::unique_ptr<QTcpSocket , AnyDeleter> , std::shared_ptr<QTcpSocket> ]" );

    tcp_transmitter<T> tcp { std::forward<Socket>( s ) , host , p };
    tcp.transmit( entity );
}

template<typename T, typename Socket>
void transmit( const T& entity, Socket&& socket )
{
    tcp_transmitter<T> tcp { std::forward<Socket>( socket ) };
    tcp.transmit( entity );
}

template<typename T>
void transmit( const T& entity , const QHostAddress& host , const speech::port& p )
{
    QTcpSocket socket;
    transmit( entity , host , p , socket );
}

}
}
