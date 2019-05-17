#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <vector>

namespace speech
{
    template<>
    class shared_socket<QTcpSocket>
    {
        public:

            shared_socket<QTcpSocket>(QTcpSocket& socket) : m_socket { socket } g
            {
                QObject::connect(&m_socket , &QTcpSocket::readyRead , [this] {
                    on_data_received();
                });
            }

            QTcpSocket& socket()
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

                if(!m_socket.bytesAvailable() && m_buffer.isEmpty())
                    return;

                //Read bytes
                m_buffer.append(m_socket.readAll());

                int number_of_bytes_processed = 0;

                //Notify listeners
                std::for_each(m_listeners.begin() , m_listeners.end() , [this , &number_of_bytes_processed](
                    std::function<int(const QByteArray&)> f){

                   auto bytes = f(m_buffer);

                   if(number_of_bytes_processed < bytes)
                        number_of_bytes_processed = bytes;
                        
                });

                if(number_of_bytes_processed > 0)
                    m_buffer.remove(0 , static_cast<int>(number_of_bytes_processed));

                // if(m_socket.bytesAvailable() || !m_buffer.isEmpty())
                //     on_data_received();

                QTimer::singleShot(0 , [this](){
                    on_data_received();
                });
            }

            QTcpSocket& m_socket;
            QByteArray m_buffer;
            std::vector<std::function<int(const QByteArray&)>> m_listeners;
    };
}