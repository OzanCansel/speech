#include <QNetworkAddressEntry>
#include "udp_transmitter.h"
#include "udp_transmitter_util.h"
#include "speech/util.h"
#include <utility>
#include <algorithm>

namespace speech
{
namespace udp
{

namespace impl
{
std::vector<QHostAddress> broadcast_addresses_of( const QNetworkInterface& iface )
{
    std::vector<QHostAddress> addresses;

    // Retrieve all broadcast addresses
    QNetworkInterface::InterfaceFlags flags = iface.flags();
    if( ( flags & QNetworkInterface::IsRunning ) && ( flags & QNetworkInterface::CanBroadcast ) ){

        for (const QNetworkAddressEntry& address : iface.addressEntries()) {

            auto broadcast_addr = address.broadcast();

            if ( broadcast_addr.isNull() )
                continue;

            addresses.push_back( broadcast_addr );

        }

    }

    // Remove duplicates
    addresses.erase( std::unique( addresses.begin() , addresses.end() ) , addresses.end() );

    return addresses;
}
}

template<typename T, typename Socket>
void transmit ( const T& entity, const QHostAddress& host, const speech::port& p, Socket&& socket )
{

    static_assert ( std::is_same_v<Socket, QUdpSocket&> ||
                    is_shared_ptr_with_v<std::decay_t<Socket> , QUdpSocket> ||
                    is_unique_ptr_with_v<std::decay_t<Socket> , QUdpSocket> ,
                    "speech::udp::transmit(const T& , const QHostAddress&, port& , Socket&& ) => Socket parameter must be one of these type : [ QUdpSocket& , std::unique_ptr<QUdpSocket , AnyDeleter> , std::shared_ptr<QUdpSocket> ]" );


    udp_transmitter<T> udp { std::forward<Socket>( socket ), host, p };

    udp.transmit ( entity );
}

template<typename T>
void transmit ( const T& entity, const QHostAddress& host, const speech::port& p )
{
    QUdpSocket socket;
    transmit ( entity , host , p , socket );
}

template<typename T>
void broadcast( const T& entity , const speech::port& p )
{
    broadcast( entity , p , QNetworkInterface::allInterfaces() );
}

template<typename T>
void broadcast( const T& entity , const speech::port& p , const QNetworkInterface& iface )
{
    auto broadcast_addresses = impl::broadcast_addresses_of( iface );

    for ( const auto& address : broadcast_addresses )
        transmit( entity , address , p );

}

template<typename T>
void broadcast( const T& entity , const speech::port& p , const QList<QNetworkInterface>& ifaces )
{
    std::vector< QHostAddress > broadcast_addresses;

    for ( const auto& iface : ifaces )
    {
        auto iface_broadcast_addresses = impl::broadcast_addresses_of( iface );

        broadcast_addresses.insert( broadcast_addresses.end() , iface_broadcast_addresses.begin() , iface_broadcast_addresses.end() );
    }

    // Remove duplicates
    broadcast_addresses.erase( std::unique( broadcast_addresses.begin() , broadcast_addresses.end() ) , broadcast_addresses.end() );

    for ( const auto& address : broadcast_addresses )
        transmit( entity , address , p );
}

} // namespace udp
} // namespace speech
