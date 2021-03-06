#include "speech/error/connection_error.h"
#include "speech/handle/unique_ptr_handle.h"
#include "speech/handle/shared_ptr_handle.h"
#include "speech/traits/is_unique_ptr.h"
#include "speech/traits/is_shared_ptr.h"
#include "tcp_transmitter.h"
#include <memory>
#include <utility>

namespace speech
{
    namespace tcp
    {

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(QTcpSocket& socket)
            :
                m_socket{ new speech::handle::handle<QTcpSocket>{ socket } }
        {   }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(const QHostAddress& host , const speech::port& p)
            :
                m_socket{ new speech::handle::unique_ptr_handle{  std::make_unique<QTcpSocket>() } }
        {   
            if(!m_socket->ref().isOpen())
                connect_to_host(host , p.get() );
        }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter(QTcpSocket& socket , const QHostAddress& host , const speech::port& p)
            :
                m_socket{ new speech::handle::handle<QTcpSocket>{socket} }
        {   
            if(!m_socket->ref().isOpen())
                connect_to_host(host , p.get());
        }

        template<typename... T>
        template<typename Deleter>
        tcp_transmitter<T...>::tcp_transmitter(std::unique_ptr<QTcpSocket , Deleter> sck)
            :
                m_socket{ new speech::handle::unique_ptr_handle{ sck } }
        {   }

        template<typename... T>
        template<typename Deleter>
        tcp_transmitter<T...>::tcp_transmitter(std::unique_ptr<QTcpSocket , Deleter>  sck , const QHostAddress& host , const speech::port&  p)
            :
                m_socket{ new speech::handle::unique_ptr_handle{ std::move(sck) } }
        {   
            if(!m_socket->ref().isOpen())
                connect_to_host(host , p.get());
        }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter( std::shared_ptr<QTcpSocket> sck )
            :
                m_socket{ new speech::handle::shared_ptr_handle<QTcpSocket>{ std::move( sck ) } }
        {   }

        template<typename... T>
        tcp_transmitter<T...>::tcp_transmitter( std::shared_ptr<QTcpSocket>  sck , const QHostAddress& host , const speech::port& p )
            :
                m_socket{ new speech::handle::shared_ptr_handle<QTcpSocket>{ std::move( sck ) } }
        {   
            if(!m_socket->ref().isOpen())
                connect_to_host(host , p.get());
        }

        template<typename... T>
        tcp_transmitter<T...>::~tcp_transmitter() noexcept
        {
            m_socket->ref().waitForBytesWritten();
        }

        template<typename... T>
        bool tcp_transmitter<T...>::write(const QByteArray& data)
        {
            auto& socket = m_socket->ref();

            if(!socket.isOpen())
                return false;

            socket.write(data);

            return true;
        }

        template<typename... T>
        void tcp_transmitter<T...>::connect_to_host(const QHostAddress& host, int port)
        {
            auto& socket = m_socket->ref();
            socket.connectToHost(host , port);

            if(!socket.waitForConnected())
                throw speech::error::connection_error{ "could not be connected." };
        }

    }
}
