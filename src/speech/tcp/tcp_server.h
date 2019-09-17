#ifndef SPEECH_TCP_SERVER_H
#define SPEECH_TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <tuple>
#include <functional>
#include <memory>
#include <utility>
#include "speech/util.h"
#include "speech/shared_socket.h"
#include "speech/tcp/tcp_receiver.h"

namespace speech
{
namespace tcp
{

template<typename T>
class handler;

namespace impl
{

struct lifetime
{

    using cb_type = std::function<int(const QByteArray& , QTcpSocket&)>;

    cb_type cb;
};

template <typename Tuple, typename F, std::size_t ...Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>);

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f);

template<typename... T , typename E , size_t... I>
auto forward_handler( std::tuple<T...>&& partial_cont , handler<E>&& rhs , std::index_sequence<I...> );

}

template<typename T>
class handler : public tcp_receiver<T>
{

public:

    using client_cb = std::function<void( const T& , QTcpSocket& )>;

    handler( client_cb&& cb , std::shared_ptr<impl::lifetime> c );
    ~handler() noexcept = default;
    handler( const handler& ) = delete;
    handler& operator = ( const handler& ) = delete;
    handler( handler&& other ) noexcept;
    handler& operator = ( handler&& other ) noexcept;

protected:

    void on_receive( const T& e ) override;

private:

    friend class tcp_server;

    std::shared_ptr<impl::lifetime> m_conn;
    client_cb m_cb;

};

class tcp_server
{
public:

    tcp_server( const QHostAddress &address = QHostAddress::Any , speech::port = speech::port(0) );
    int port() const;

    template<typename... T>
    tcp_server& listen( const std::tuple<handler<T>...>& handlers );

    template<typename T>
    tcp_server& listen( const handler<T>& handler );

private:

    int ready_read_callback( const QByteArray& , QTcpSocket& );
    void new_connection();
    void disconnected ( QTcpSocket* );
    std::vector<shared_socket<QTcpSocket>>  m_alive_connections;
    std::vector<std::shared_ptr<impl::lifetime>> m_lifetimes;
    QTcpServer m_server;

};

template<typename T>
handler<T> listen( typename handler<T>::client_cb&& cb );

template< typename L , typename R >
std::tuple< handler<L> , handler<R> > operator | ( handler<L>&& lhs , handler<R>&& rhs );

template< typename... L , typename R >
auto operator | ( std::tuple<L...>&& lhs , handler<R>&& rhs );

}
}

#include "tcp_server_impl.h"

#endif
