#ifndef SPEECH_TCP_SERVER_H
#define SPEECH_TCP_SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <unordered_map>
#include <tuple>
#include <functional>
#include "speech/util.h"
#include "speech/shared_socket.h"
#include "speech/handler.h"

namespace speech
{
    namespace tcp
    {

        template<typename... T>
        class tcp_server
        {
            public:

                tcp_server(const QHostAddress &address = QHostAddress::Any, speech::port = speech::port(0) , T...);

            private:

                using handlers_cont = std::tuple<typename T::result_type...>;

                void new_connection();
                void disconnected(QTcpSocket* s);
                std::tuple<T...> m_factory;
                std::unordered_map<std::unique_ptr<shared_socket<QTcpSocket>> , handlers_cont>  m_alive_connections;
                QTcpServer m_server;
        };

        template<typename T , typename... Ps>
        std::function<std::unique_ptr<T>(shared_socket<QTcpSocket>&)> make_handler(Ps... values)
        {
            std::function<std::unique_ptr<T>(shared_socket<QTcpSocket>&)> f = [values...](shared_socket<QTcpSocket>& sck){ return std::unique_ptr<T>{ new T{ sck , values... } }; };
            return f;
        }

        template<typename Entity>
        std::function<std::unique_ptr<speech::handler_f<Entity , tcp_receiver>>(shared_socket<QTcpSocket>&)> make_handler_f(std::function<void(const Entity& , QTcpSocket&)> f)
        {
            using namespace impl;
            return [f](shared_socket<QTcpSocket>& sck){ 
                return std::make_unique<handler_f<Entity , tcp_receiver>>(  f , sck ); 
            };
        }


        template<typename... T>
        auto make_server(const QHostAddress &address = QHostAddress::Any, speech::port p = speech::port(0) , T... generators) -> std::unique_ptr<tcp_server<decltype(generators)...>>
        {
            using server_type = tcp_server<decltype(generators)...>;
            return std::unique_ptr<server_type>( new server_type(address , p , generators...));
        }
    }
}

#include "tcp_server_impl.h"

#endif