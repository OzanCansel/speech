#include "speech/error/connection_error.h"

namespace speech
{
    namespace tcp
    {
        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(QHostAddress host , speech::port p)
            :
                tcp_transmitter<T...>{ m_default , host , p }
        {   }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(QTcpSocket& socket)
        {   
            if(!m_socket.isOpen())
                connect_to_host();
        }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(QTcpSocket& socket , QHostAddress host , speech::port p)
            :
                m_socket{ socket },
                m_host{ host },
                m_port{ p.get() }
        {   
            if(!m_socket.isOpen())
                connect_to_host();
        }

        template<typename... T>
        bool tcp_transmitter<T...>::write(const QByteArray& data)
        {
            if(!m_socket.isOpen())
                return false;

            m_socket.write(data);
            m_socket.waitForBytesWritten();

            return true;
        }

        template<typename... T>
        void tcp_transmitter<T...>::connect_to_host()
        {
            m_socket.connectToHost(m_host , m_port);

            if(!m_socket.waitForConnected())
                throw speech::error::connection_error{ "could not be connected." };
        }
    }
}