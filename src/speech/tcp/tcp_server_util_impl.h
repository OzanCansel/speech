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

template<typename T , typename Container>
struct forwarder_sad
{

    void operator()( const T&e , std::weak_ptr<QTcpSocket> s ) { m_cont( e , s ); }

    forwarder_sad( Container& cont ) : m_cont { cont } { }

    Container& m_cont;
};

template<typename Observer, typename... Entities>
inline auto redirect_impl( Observer& cont)
{
    return ( (listen<Entities>( forwarder_sad< Entities , Observer > ( cont ) )) | ... );
}

template<typename Ret, typename Arg , typename Arg2>
Arg argument_type(Ret(*)(Arg , Arg2));

template<typename Ret, typename Fn, typename Arg , typename Arg2 >
Arg argument_type(Ret(Fn::*)(Arg , Arg2) const);

template<typename Fn>
auto argument_type(Fn) -> decltype(argument_type(&Fn::operator()));

template<typename Arg, typename Fn, typename Arg2>
struct argument {
    static_assert(std::is_invocable_v<Fn, Arg , Arg2 >);
    using type = Arg;
};

template<typename Fn>
struct argument<void, Fn , std::weak_ptr< QTcpSocket > >{
    using type = decltype(argument_type(std::declval<Fn>()));
};

}

template<typename... Entities, typename Observer>
inline auto redirect ( Observer& to )
{
    static_assert( sizeof...( Entities ) > 0 , "You should specify at least one Entity type." );
    return impl::redirect_impl<Observer , Entities...>( to );
}

template<typename T, typename Fn>
inline auto listen(Fn fn) {
    using Arg = std::decay_t<typename impl::argument<T, Fn , std::weak_ptr<QTcpSocket>>::type>;
    return handler<Arg> {  std::forward<typename handler<Arg>::client_cb>( fn ) , std::make_shared<impl::lifetime>() };
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

