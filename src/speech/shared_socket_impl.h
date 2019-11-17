#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <vector>
#include <utility>
#include <memory>
#include "shared_socket.h"
#include "speech/util.h"

namespace speech
{
    template<>
    class shared_socket<QTcpSocket>
    {
        public:

        using observer_cb_t = std::function<int(const QByteArray& , std::weak_ptr<QTcpSocket> )>;

            shared_socket<QTcpSocket>( std::shared_ptr<QTcpSocket>&& socket )
                :   m_socket { std::forward<std::shared_ptr<QTcpSocket>>( socket ) }
            {
                listen();
            }

            shared_socket<QTcpSocket>( std::unique_ptr<QTcpSocket , socket_deleter>&& socket )
                : m_socket { std::shared_ptr<QTcpSocket> { std::move( socket ) } }
            {
                listen();
            }

            ~shared_socket() noexcept
            {
                QObject::disconnect( m_socket_listening_conn );
                m_socket->disconnect();
            }

            std::weak_ptr<QTcpSocket> socket()
            {
                return m_socket;
            }

            void attach( observer_cb_t&& observer )
            {
                m_listeners.push_back( std::forward<observer_cb_t>( observer ) );
            }

            shared_socket<QTcpSocket>(const shared_socket<QTcpSocket>&) = delete;
            shared_socket<QTcpSocket>& operator=(const shared_socket<QTcpSocket>&) = delete;
            shared_socket<QTcpSocket>( shared_socket<QTcpSocket>&& rhs ) noexcept
            {
                m_socket = std::move(rhs.m_socket);
                m_buffer = std::move(rhs.m_buffer);
                m_listeners = std::move(rhs.m_listeners);
                listen();
            }

            shared_socket<QTcpSocket>& operator=(shared_socket<QTcpSocket>&& rhs) noexcept
            {
                m_socket = std::move(rhs.m_socket);
                m_buffer = std::move(rhs.m_buffer);
                m_listeners = std::move(rhs.m_listeners);
                listen();

                return *this;
            }

        private:

            void listen()
            {
                m_socket_listening_conn = QObject::connect(m_socket.get() , &QTcpSocket::readyRead , m_socket.get() , [this] {
                    on_data_received();
                });
            }

            void on_data_received()
            {

                if ( !m_socket )
                    return;

                auto& sck = *m_socket.get();
                int parsed_data_length{};
                do
                {

                    //Read bytes
                    m_buffer.append(sck.readAll());

                    bool invalid_msg { false };
                    int could_not_be_parsed_count {};
                    parsed_data_length = 0;

                    //Call listeners
                    for( auto& f : m_listeners )
                    {
                        auto processed_bytes = f ( m_buffer , m_socket );

                        if(processed_bytes == -1)
                        {
                            could_not_be_parsed_count++;
                        }
                        else if(processed_bytes == -2)
                        {
                            invalid_msg = true;
                            break;
                        }
                        else if(processed_bytes > parsed_data_length)
                        {
                            parsed_data_length = processed_bytes;
                        }
                    }

                    auto could_not_be_parsed = could_not_be_parsed_count == m_listeners.size();

                    const static QByteArray start_token = [](){
                        QByteArray arr;
                        QDataStream ss(&arr , QIODevice::WriteOnly);
                        ss.setVersion(QDataStream::Qt_5_0);
                        ss << 241994 << 1511999 << 991973;
                        return arr;
                    }();

                    if(invalid_msg || could_not_be_parsed)
                    {

                        int idx = -1;
                        idx = m_buffer.indexOf( start_token );

                        if ( idx == 0 )
                        {
                            m_buffer.remove( 0 , start_token.size() );
                            idx = m_buffer.indexOf( start_token );
                        }

                        //discard current message and jump to next message
                        auto invalid_data_len = idx == -1 ? m_buffer.size() : idx;
                        m_buffer.remove(0 , invalid_data_len);
                    }
                    else
                    {
                        m_buffer.remove(0 , parsed_data_length);
                    }

                } while((!m_buffer.isEmpty() && parsed_data_length > 0) || sck.bytesAvailable());
            }

            std::shared_ptr<QTcpSocket> m_socket;
            QByteArray m_buffer;
            std::vector< observer_cb_t > m_listeners;
            QMetaObject::Connection m_socket_listening_conn;
    };
}
