#ifndef SPEECH_TCP_ASK_H
#define SPEECH_TCP_ASK_H

#include "tcp_receiver.h"
#include "tcp_transmitter.h"
#include <variant>
#include <chrono>
#include <future>
#include <type_traits>
#include <QTcpSocket>

namespace speech
{
namespace tcp
{

struct not_responding : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

namespace impl
{

template< typename Variant , typename Receiver , typename HeadResult , size_t... Indices >
bool assign_if_any( Variant& v , Receiver& rec , std::index_sequence< 0 > )
{
    auto msgs = rec.messages( speech::impl::identifier< HeadResult > {} );
    if ( ! msgs.empty() )
    {
        v = msgs.front();
        return true;
    }

    return false;
}

template< typename Variant , typename Receiver , typename HeadResult , typename... TailPossibleResults , size_t... Indices>
bool assign_if_any( Variant& v , Receiver& rec , std::index_sequence< Indices... > )
{
    auto msgs = rec.messages( speech::impl::identifier< HeadResult > {} );
    if ( ! msgs.empty() )
    {
        std::get< HeadResult >( v ) = msgs.front();
        return true;
    }

    return assign_if_any< Variant , Receiver , TailPossibleResults... >( v , rec , std::index_sequence_for< TailPossibleResults...>{} );
}

template<typename... PossibleResults>
struct ask_impl
{
    template<typename Question >
    std::variant< PossibleResults... > operator()( const Question& q , const std::chrono::milliseconds& timeout )
    {
        return ask( q , timeout );
    }

    template< typename Question >
    std::variant< PossibleResults... > ask( const Question& q , const std::chrono::milliseconds& timeout )
    {

        std::variant< PossibleResults... > result;
        queued_tcp_receiver< PossibleResults... > receiver { m_sck };
        {
            tcp_transmitter< Question > t { *m_sck };
            t.transmit( q );
        }

        bool responded {};
        auto starting_point = std::chrono::high_resolution_clock::now();

        do
        {
            receiver.wait_for_receive( 1 );
            responded = assign_if_any< decltype( result ) , decltype( receiver ) , PossibleResults... >( result , receiver , std::index_sequence_for< PossibleResults... > { } );
        }
        while ( std::chrono::high_resolution_clock::now() - starting_point < timeout && !responded );

        if ( !responded )
            throw not_responding { std::string{ m_sck->peerAddress().toString().toStdString() } + std::string { ":" } + std::to_string( m_sck->peerPort() ) + std::string{ } };

        return result;
    }

    ask_impl( const std::shared_ptr<QTcpSocket>& sck )
        : m_sck { sck }
    { }

private:

    std::shared_ptr<QTcpSocket> m_sck;

};

}

template< typename T >
struct response_of {};

template<typename... PossibleResults , typename Question , typename = std::enable_if_t< ( sizeof...( PossibleResults ) > 0 ) >>
std::variant< PossibleResults... > ask( const Question& q , std::shared_ptr< QTcpSocket > sck , const std::chrono::milliseconds& timeout)
{
    return impl::ask_impl< PossibleResults... >{ sck }( q , timeout );
}

// If expected results are not specified, then default as response_of template
template<typename... PossibleResults , typename Question , typename = std::enable_if_t< ( sizeof...( PossibleResults ) == 0 ) >>
response_of< Question > ask( const Question& q , std::shared_ptr< QTcpSocket > sck , const std::chrono::milliseconds& timeout)
{
    return std::get<0>( impl::ask_impl< response_of< Question > >{ sck }( q , timeout ) );
}

template< typename Expected , typename Response >
constexpr bool responded_with( const Response& r )
{
    return std::holds_alternative<Expected>( r );
}

template< typename Response >
constexpr bool responded_with( const Response& r )
{
    return std::holds_alternative< response_of< Response > >( r );
}

}
}

#endif
