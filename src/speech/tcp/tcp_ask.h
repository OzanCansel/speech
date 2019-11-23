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

template< typename T >
struct response_of {};

// General ask
template<typename... PossibleResults , typename Question , typename = std::enable_if_t< ( sizeof...( PossibleResults ) > 0 ) >>
std::variant< PossibleResults... > ask( const Question& q , std::shared_ptr< QTcpSocket > sck , const std::chrono::milliseconds& timeout = std::chrono::milliseconds { 1000 } );

// If expected results are not specified, then default as response_of template
template<typename... PossibleResults , typename Question , typename = std::enable_if_t< ( sizeof...( PossibleResults ) == 0 ) >>
response_of< Question > ask( const Question& q , std::shared_ptr< QTcpSocket > sck , const std::chrono::milliseconds& timeout = std::chrono::milliseconds { 1000 } );

template< typename Expected , typename Response >
constexpr bool responded_with( const Response& r ) noexcept;

}
}

#include "tcp_ask_impl.h"

#endif
