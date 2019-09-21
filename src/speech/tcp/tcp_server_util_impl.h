#include "tcp_server_util.h"

namespace speech
{
namespace tcp
{

namespace impl
{

template<typename... LTuple, size_t... LIdx , typename... RTuple, size_t... RIdx>
inline auto combine_tuples( std::tuple<LTuple...>&& l_tuple , std::index_sequence<LIdx...>, std::tuple<RTuple...>&& r_tuple , std::index_sequence<RIdx...>)
{
    using namespace std;
    return make_tuple( forward< LTuple >( get<LIdx>( l_tuple ) )... , forward<RTuple>( get<RIdx>( r_tuple ) )... );
}

template<size_t , typename Observer , typename Entity>
inline auto redirect_impl( Observer& cont , speech::impl::specializer<0> )
{
    return listen<Entity>( [&cont]( const Entity& e , std::weak_ptr<QTcpSocket> s ) { cont( e , s ); } );
}

template<size_t I , typename Observer , typename Entity , typename... Rest>
inline auto redirect_impl ( Observer& cont , speech::impl::specializer<I> )
{
    return listen<Entity>( [&cont]( const Entity& e , std::weak_ptr<QTcpSocket> s ) { cont( e , s ); } ) |
            redirect_impl<I - 1 , Observer , Rest...>( cont , speech::impl::specializer< I - 1 >{});
}

}

template<typename... Entities, typename Observer>
inline auto redirect ( Observer& to )
{
    static_assert( sizeof...( Entities ) > 0 , "You should specify at least one Entity type." );
    return impl::redirect_impl<sizeof...(Entities) - 1 , Observer , Entities...>( to , speech::impl::specializer<sizeof...(Entities) - 1>{ });
}

template<typename T>
inline handler<T> listen( typename handler<T>::client_cb&& cb )
{
    return handler<T> {  std::forward<typename handler<T>::client_cb>( cb ) , std::make_shared<impl::lifetime>() };
}

template< typename L , typename R >
inline auto operator | ( handler<L>&& lhs , handler<R>&& rhs )
{
    using namespace std;
    return impl::combine_tuples( std::make_tuple( forward<handler<L>> ( lhs ) ) , std::make_index_sequence< 1 >{ } , std::make_tuple( forward<handler<R>> ( rhs ) ) , std::make_index_sequence< 1 >{ } );
}

template< typename... L , typename R >
inline auto operator | ( std::tuple< handler<L>... >&& lhs , handler<R>&& rhs )
{
    using namespace std;
    return impl::combine_tuples( forward<std::tuple< handler<L>... >>( lhs ) , std::index_sequence_for< L... >{} , std::make_tuple( forward<handler<R>> ( rhs ) ) , std::make_index_sequence< 1 >{ } );
}

template< typename... L , typename... R >
inline auto operator | ( std::tuple< handler<L>... >&& lhs , std::tuple< handler<R>... >&& rhs )
{
    using namespace std;
    return impl::combine_tuples( forward<std::tuple< handler<L>... >>( lhs ), std::index_sequence_for< L... >{} , forward<std::tuple< handler<R>... >>( rhs ) , std::index_sequence_for< R... > { } );
}

}
}

