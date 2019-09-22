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

    using cb_type = std::function<int(const QByteArray& , std::weak_ptr<QTcpSocket> )>;

    cb_type cb;
};

template <typename Tuple, typename F, std::size_t ...Indices>
inline void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>);

template <typename Tuple, typename F>
inline void for_each(Tuple&& tuple, F&& f);

}

template <typename... T>
class tcp_server_data_parser : protected receiver<false, T...>
{
public:

     using socket_type = QTcpSocket;
     int on_data_received ( const QByteArray & , std::weak_ptr<QTcpSocket>&& );

     std::weak_ptr<QTcpSocket> socket() { return m_sck; }

protected:

     inline QTcpSocket &device();

private:

     std::weak_ptr<QTcpSocket> m_sck {  };

};

template<typename T>
class handler : public tcp_server_data_parser<T>
{

public:

    using client_cb = std::function<void( const T& , std::weak_ptr<QTcpSocket> )>;

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

    inline tcp_server( const QHostAddress &address = QHostAddress::Any , speech::port = speech::port(0) );
    inline int port() const;

    template<typename... T>
    inline tcp_server& listen( const std::tuple<handler<T>...>& handlers );

    template<typename T>
    inline tcp_server& listen( const handler<T>& handler );

    template<typename T>
    inline void broadcast( const T& );

    inline void disconnect_socket ( std::weak_ptr<QTcpSocket> sck );

private:

    inline int ready_read_callback( const QByteArray& , std::weak_ptr<QTcpSocket> );
    inline void new_connection();
    inline void disconnected ( QTcpSocket* );
    std::vector<shared_socket<QTcpSocket>>  m_alive_connections;
    std::vector<std::shared_ptr<impl::lifetime>> m_lifetimes;
    QTcpServer m_server;

};

}
}

#include "tcp_server_impl.h"

#endif
