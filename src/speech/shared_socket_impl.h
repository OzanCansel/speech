#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <vector>
#include <memory>
#include "shared_socket.h"

namespace speech
{
    template<>
    class shared_socket<QTcpSocket>
    {
        public:

            shared_socket<QTcpSocket>(QTcpSocket& socket) 
                : 
                m_socket { new speech::handle::handle<QTcpSocket>{ socket } }
            { 
                listen();
            }

            shared_socket<QTcpSocket>(std::shared_ptr<QTcpSocket> socket)
                :
                m_socket{ new speech::handle::shared_ptr_handle<QTcpSocket>{ socket }}
            {   
                listen();
            }

            shared_socket<QTcpSocket>(std::unique_ptr<QTcpSocket> socket)
                :
                m_socket{ new speech::handle::unique_ptr_handle{ std::move(socket) }}
            {   
                listen();
            }

            ~shared_socket() noexcept
            {
                QObject::disconnect( m_socket_listening_conn );
            }

            QTcpSocket& socket()
            {
                return m_socket->ref();
            }

            void attach(std::function<int(const QByteArray& , QTcpSocket&)> observer)
            {
                m_listeners.push_back( observer );
            }

            shared_socket<QTcpSocket>(const shared_socket<QTcpSocket>&) = delete;
            shared_socket<QTcpSocket>& operator=(const shared_socket<QTcpSocket>&) = delete;
            shared_socket<QTcpSocket>(shared_socket<QTcpSocket>&& rhs)
            {
                m_socket = std::move(rhs.m_socket);
                m_buffer = std::move(rhs.m_buffer);
                m_listeners = std::move(rhs.m_listeners);
                listen();
            }

            shared_socket<QTcpSocket>& operator=(shared_socket<QTcpSocket>&& rhs)
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
                m_socket_listening_conn = QObject::connect(&m_socket->ref() , &QTcpSocket::readyRead , [this] {
                    on_data_received();
                });
            }

            void on_data_received()
            {
                auto& sck = m_socket->ref();
                int parsed_data_length{};
                do
                {

                    //Read bytes
                    m_buffer.append(sck.readAll());

                    bool invalid_msg { false };
                    int could_not_be_parsed_count {};
                    parsed_data_length = 0;

                    //Call listeners
                    for(auto f : m_listeners)
                    {
                        auto processed_bytes = f ( m_buffer , m_socket->ref() );

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

                        if(could_not_be_parsed)
                            m_buffer.remove(0 , start_token.size());

                        //discard current message and jump to next message
                        auto start_of_msg_idx = m_buffer.indexOf(start_token);
                        auto invalid_data_len = start_of_msg_idx == -1 ? m_buffer.size() : start_of_msg_idx;
                        m_buffer.remove(0 , invalid_data_len);
                    }
                    else
                    {
                        m_buffer.remove(0 , parsed_data_length);
                    }

                } while((!m_buffer.isEmpty() && parsed_data_length > 0) || sck.bytesAvailable());
            }

            std::unique_ptr<speech::handle::handle<QTcpSocket>> m_socket;
            QByteArray m_buffer;
            std::vector<std::function<int(const QByteArray& , QTcpSocket& )>> m_listeners;
            QMetaObject::Connection m_socket_listening_conn;
    };
}
