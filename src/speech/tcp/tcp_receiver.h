#ifndef SPEECH_TCP_RECEIVER_H
#define SPEECH_TCP_RECEIVER_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QByteArray>
#include "speech/receiver.h"
#include "speech/util.h"
#include "speech/shared_socket.h"

namespace speech
{
    namespace tcp
    {

        template<typename... T>
        class tcp_receiver : protected receiver<T...>
        {
            public:

            using socket_type = QTcpSocket;
            using shared_socket_Type = shared_socket<socket_type>;

            tcp_receiver(shared_socket<QTcpSocket>&);

            private:

            int on_data_received(const QByteArray&);

        };
    }
}

#include "tcp_receiver_impl.h"

#endif