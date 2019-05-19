#ifndef SPEECH_TCP_TRANSMITTER_H
#define SPEECH_TCP_TRANSMITTER_H

#include <QTcpSocket>
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
                tcp_transmitter(std::unique_ptr<QTcpSocket>);
                tcp_transmitter(std::unique_ptr<QTcpSocket> , const QHostAddress&, const speech::port&);
                tcp_transmitter(std::shared_ptr<QTcpSocket>);
                tcp_transmitter(std::shared_ptr<QTcpSocket> , const QHostAddress&, const speech::port&);

            protected:

                bool write(const QByteArray&) override;

            private:

                void connect_to_host(const QHostAddress& , int);
                
                std::unique_ptr<speech::handle::handle<QTcpSocket>>  m_socket;
        };
    }
}

#include "tcp_transmitter_impl.h"

#endif