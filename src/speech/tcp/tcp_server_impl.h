#include "speech/error/connection_error.h"
#include "speech/error/port_in_use_error.h"
#include "speech/util.h"
#include <QObject>
#include <utility>

namespace speech
{
    namespace tcp
    {

        namespace impl
        {
            template<typename Cont, typename G, size_t... I>
            void insert(Cont& c, G& g , QTcpSocket& socket , std::index_sequence<I...>)
            {
                using namespace std;
                auto shared_s = unique_ptr<shared_socket<QTcpSocket>>(new speech::shared_socket<QTcpSocket>( socket ));
                auto& s = *shared_s.get();
                c[move(shared_s)] = make_tuple(
                    (get<I>(g)(s))...);
            }
        }

        template<typename... T>
        tcp_server<T...>::tcp_server(const QHostAddress &address, speech::port p , T... generators)
            :
                m_factory{ generators... }
        {
            using namespace speech::error;

            if(!m_server.listen(address , p.get()))
            {
                if(m_server.serverError() == QAbstractSocket::AddressInUseError)
                    throw port_in_use_error{ m_server.errorString().toStdString() };

                throw std::runtime_error { m_server.errorString().toStdString() };
            }

            QObject::connect(&m_server , &QTcpServer::newConnection , [this]{ new_connection(); });
        }

        template<typename... T>
        void tcp_server<T...>::new_connection()
        {
             //Fork new receiver and push to receivers list
             auto socket= m_server.nextPendingConnection();
            // insert(m_alive_connections , m_factory, *socket , sequence_t<sizeof...(T)>{});
            impl::insert(m_alive_connections , m_factory, *socket , std::make_index_sequence<sizeof...(T)>());

            QObject::connect(socket , &QTcpSocket::disconnected , [this, socket]() {
                disconnected(socket);
            });

            QObject::connect(socket , &QTcpSocket::destroyed , [this, socket]() {
                destroyed(socket);
            });
        }

        template<typename... T>
        void tcp_server<T...>::disconnected(QTcpSocket* socket)
        {
            socket->deleteLater();
        }

        template<typename... T>
        void tcp_server<T...>::destroyed(QTcpSocket* socket)
        {
            auto entry = std::find_if(m_alive_connections.begin() , m_alive_connections.end(), [&socket](const auto& shared_socket)
            {
                return &shared_socket.first->socket() == socket;
            });

            if(entry != m_alive_connections.end())
                m_alive_connections.erase(entry);
        }
    }
}