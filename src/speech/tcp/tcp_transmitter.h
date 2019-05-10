#ifndef SPEECH_TCP_TRANSMITTER_H
#define SPEECH_TCP_TRANSMITTER_H

#include <QTcpSocket>
#include "speech/transmitter.h"

namespace speech
{
    namespace tcp
    {
        template<typename... T>
        class tcp_transmitter : public transmitter<T...>
        {
            public:

                tcp_transmitter(QHostAddress, speech::port);
                tcp_transmitter(QTcpSocket&);
                tcp_transmitter(QTcpSocket&, QHostAddress, speech::port);

            protected:

                bool write(const QByteArray&) override;

            private:

                void connect_to_host();
                
                QTcpSocket m_default;
                QTcpSocket& m_socket;

                QHostAddress m_host;
                int m_port {};

        };
    }
}

#include "tcp_transmitter_impl.h"

#endif