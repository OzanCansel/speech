#ifndef SPEECH_TCP_SERVER_H
#define SPEECH_TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <unordered_map>
#include <tuple>
#include <functional>
#include <memory>
#include "speech/util.h"
#include "speech/shared_socket.h"
#include "speech/tcp/tcp_receiver.h"

namespace speech
{
namespace tcp
{

namespace impl
{

struct lifetime
{

    using cb_type = std::function<int(const QByteArray& , QTcpSocket&)>;

    cb_type cb;
};
}

template<typename T>
class handler : public tcp_receiver<T>
{

public:

    using client_cb = std::function<void( const T& , QTcpSocket& )>;

    handler( client_cb cb , std::shared_ptr<impl::lifetime> c )
        : m_conn { c }
        , m_cb { cb }
    {
        using namespace std::placeholders;
        c->cb = std::bind( &tcp_receiver<T>::on_data_received , this , _1 , _2 );
    }

    handler( const handler& ) = delete;
    handler& operator = ( const handler& ) = delete;
    handler( handler&& other ) noexcept
    {
        using namespace std::placeholders;

        m_conn = other.m_conn;
        m_cb = other.m_cb;
        m_conn->cb = std::bind( &tcp_receiver<T>::on_data_received , this , _1 , _2 );
    }

    handler& operator = ( handler&& other )
    {
        using namespace std::placeholders;

        m_conn = other.m_conn;
        m_cb = other.m_cb;
        m_conn->cb = std::bind( tcp_receiver<T>::on_data_received , this , _1 , _2 );
    }

protected:

    void on_receive( const T& e )
    {
        m_cb( e , tcp_receiver<T>::socket() );
    }

private:

    friend class tcp_server;

    std::shared_ptr<impl::lifetime> m_conn;
    client_cb m_cb;

};

template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>) {
    using swallow = int[];
    (void)swallow{1,
        (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})...
    };
}

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f) {
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f),
                  std::make_index_sequence<N>{});
}

class tcp_server
{
public:

    tcp_server(const QHostAddress &address = QHostAddress::Any, speech::port = speech::port(0));
    int port() const { return m_server.serverPort(); }

    template<typename... T>
    tcp_server& operator | ( std::tuple<handler<T>...>& handlers )
    {
        for_each( handlers , [ this ]( auto& handler ) { m_lifetimes.push_back( handler.m_conn ); } );

        return *this;
    }

    template<typename... T>
    tcp_server& listen( std::tuple<handler<T>...>& handlers )
    {
        for_each( handlers , [ this ]( auto& handler ) { m_lifetimes.push_back( handler.m_conn ); } );

        return *this;
    }

    template<typename T>
    tcp_server& listen( handler<T>& handler )
    {
        m_lifetimes.push_back( handler.m_conn );
        return *this;
    }

private:

    int ready_read_callback( const QByteArray& , QTcpSocket& );
    void new_connection();
    void disconnected ( QTcpSocket* s );
    std::vector<shared_socket<QTcpSocket>>  m_alive_connections;
    std::vector<std::shared_ptr<impl::lifetime>> m_lifetimes;
    QTcpServer m_server;

};

template<typename T>
handler<T> listen( typename handler<T>::client_cb cb )
{
    return handler<T> {  cb , std::make_shared<impl::lifetime>() };
}

template< typename L , typename R >
std::tuple< handler<L> , handler<R> > operator | ( handler<L>&& lhs , handler<R>&& rhs )
{
    return std::make_tuple( std::forward<handler<L>>( lhs ) , std::forward<handler<R>>( rhs ) );
}

template<typename... T , typename C , size_t... I>
auto forward_handler( std::tuple<T...>&& partial_cont , handler<C>&& rhs , std::index_sequence<I...> )
{
    using namespace std;

    return tuple<T... , handler<C>>( move( get<I>( partial_cont ) )... , forward<handler<C>>( rhs ));
}

template< typename... L , typename R >
auto operator | ( std::tuple<L...>&& lhs , handler<R>&& rhs )
{
    using namespace std;
    return forward_handler( forward<tuple<L...>>( lhs ) ,  forward<handler<R>>( rhs ), std::index_sequence_for< L... >() );
}

}
}

#include "tcp_server_impl.h"

#endif
