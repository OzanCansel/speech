#ifndef SPEECH_TCP_TRANSMITTER_H
#define SPEECH_TCP_TRANSMITTER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <memory>
#include "speech/util.h"
#include "speech/transmitter.h"
#include "speech/handle/handle.h"

namespace speech
{
    namespace tcp
    {
        template<typename... T>
        class tcp_transmitter : public transmitter<T...>
        {
            public:

                tcp_transmitter(QTcpSocket&);
                tcp_transmitter(const QHostAddress&, const speech::port&);
                tcp_transmitter(QTcpSocket&, const QHostAddress&, const speech::port&);

                template<typename Deleter>
                tcp_transmitter(std::unique_ptr<QTcpSocket , Deleter>);
                template<typename Deleter>
                tcp_transmitter(std::unique_ptr<QTcpSocket , Deleter> , const QHostAddress&, const speech::port&);
                tcp_transmitter(std::shared_ptr<QTcpSocket>);
                tcp_transmitter(std::shared_ptr<QTcpSocket> , const QHostAddress&, const speech::port&);
                ~tcp_transmitter() noexcept;
                tcp_transmitter(const tcp_transmitter<T...>&) = delete;
                tcp_transmitter<T...>& operator=(const tcp_transmitter<T...>&) = delete;
                tcp_transmitter<T...>& operator=( tcp_transmitter<T...>&& ) noexcept = default;
                tcp_transmitter<T...>( tcp_transmitter<T...>&& ) noexcept = default;

            protected:

                bool write(const QByteArray&) override;

            private:

                void connect_to_host(const QHostAddress& , int);
                
                std::unique_ptr<speech::handle::handle<QTcpSocket>>  m_socket;
        };

        template<typename T, typename Socket>
        void transmit( const T& , const QHostAddress&, const speech::port& , Socket&& );

        template<typename T, typename Socket>
        void transmit( const T&, Socket&& );
        
        template<typename T>
        void transmit( const T& , const QHostAddress& , const speech::port& );
    }
}

#include "tcp_transmitter_impl.h"

#endif
