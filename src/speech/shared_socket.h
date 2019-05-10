#ifndef SPEECH_SHARED_SOCKET_H
#define SPEECH_SHARED_SOCKET_H

#include <QTcpSocket>

namespace speech
{
        template<typename T>
        class shared_socket
        {
            // static_assert(false , "T socket is not implemented yet.");
        };

        template<>
        class shared_socket<QTcpSocket>;
}

#include "shared_socket_impl.h"

#endif