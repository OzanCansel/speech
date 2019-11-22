#ifndef SPEECH_TCP_SERVER_UTIL_H
#define SPEECH_TCP_SERVER_UTIL_H

#include "tcp_server.h"

namespace speech
{
namespace tcp
{

namespace impl
{

template<typename... LTuple, size_t... LIdx , typename... RTuple, size_t... RIdx>
inline auto combine_tuples( std::tuple<LTuple...>&& , std::index_sequence<LIdx...>, std::tuple<RTuple...>&& , std::index_sequence<RIdx...>);

template<size_t , typename Observer , typename Entity>
inline auto redirect_impl( Observer& cont , speech::impl::specializer<0> );

template<size_t I , typename Observer , typename Entity , typename... Rest>
inline auto redirect_impl ( Observer& cont , speech::impl::specializer<I> );

}

template<typename... Entities, typename Observer>
inline auto redirect ( Observer& to );

template<typename T>
inline handler<T> listen( void (*cb)( const T& , std::weak_ptr<QTcpSocket> ) );

template< typename L , typename R >
inline auto operator | ( handler<L>&& lhs , handler<R>&& rhs );

template< typename... L , typename R >
inline auto operator | ( std::tuple< handler<L>...>&& lhs , handler<R>&& rhs );

template< typename... L , typename... R >
inline auto operator | ( std::tuple< handler<L>... >&& lhs , std::tuple< handler<R>... >&& rhs );

}
}

#include "tcp_server_util_impl.h"

#endif
