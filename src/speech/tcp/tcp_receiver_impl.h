#include "speech/error/connection_error.h"

namespace speech
{
    namespace tcp
    {
        
        template<typename... T>
        tcp_receiver<T...>::tcp_receiver(shared_socket<QTcpSocket>& shared_sck)
            :
                m_socket{ shared_sck.socket() }
        {
            using namespace std::placeholders;
            shared_sck.attach(std::bind(&tcp_receiver<T...>::on_data_received , this , _1));
        }

        template<typename... T>
        QTcpSocket& tcp_receiver<T...>::socket()
        {
            return m_socket;
        }

        template<typename... T>
        int tcp_receiver<T...>::on_data_received(const QByteArray& buffer)
        {
            return this->parse(buffer);
        }
    }
}