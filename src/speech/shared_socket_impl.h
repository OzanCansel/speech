#include <QObject>
#include <QByteArray>
#include <QDataStream>
#include <vector>
#include <QDebug>

namespace speech
{
    template<>
    class shared_socket<QTcpSocket>
    {
        public:

            shared_socket<QTcpSocket>(QTcpSocket& socket) : m_socket { socket } { 
                QObject::connect(&m_socket , &QTcpSocket::readyRead , [this] {
                    on_data_received();
                });
            }

            inline  QTcpSocket& socket()
            {
                return m_socket;
            }

            void attach(std::function<int(const QByteArray&)> observer)
            {
                m_listeners.push_back(observer);
            }

        private:

            void on_data_received()
            {
                do
                {

                    //Read bytes
                    m_buffer.append(m_socket.readAll());

                    bool invalid_msg{ false };
                    int could_not_be_parsed_count{};
                    int parsed_data_length{ };

                    //Call listeners
                    for(auto f : m_listeners)
                    {
                        auto processed_bytes = f(m_buffer);

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

                } while(!m_buffer.isEmpty() || m_socket.bytesAvailable());
            }

            QTcpSocket& m_socket;
            QByteArray m_buffer;
            std::vector<std::function<int(const QByteArray&)>> m_listeners;
    };
}