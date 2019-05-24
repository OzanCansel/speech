#ifndef SPEECH_FELLOW_H
#define SPEECH_FELLOW_H

#include <QTcpSocket>
#include <QHostAddress>
#include <QDataStream>
#include <QDebug>
#include <memory>
#include "speech/util.h"
#include "speech/handle/unique_ptr_handle.h"
#include "speech/tcp/tcp_transmitter.h"
#include "speech/tcp/tcp_receiver.h"

namespace speech
{
    namespace chit_chat
    {

        template<typename T>
        QDataStream& operator<<(QDataStream& ss, const T&)
        {
            return ss;
        }

        template<typename T>
        QDataStream& operator>>(QDataStream& ss,T&)
        {
            return ss;
        }

        template<typename T>
        QDebug& operator<<(QDebug out, const T&)
        {
            return out << QString::fromStdString(typeid(T).name());
        }

        template<typename T>
        struct answer
        {   };

        struct fellow
        {

                fellow(const QHostAddress& , const speech::port&)
                    :
                    m_socket{ new speech::handle::unique_ptr_handle<QTcpSocket>{ std::make_unique<QTcpSocket>() } }
                {   }

                template<typename T>
                answer<T> operator,(const T& question)
                {
                    using namespace speech::tcp;
                    shared_socket<QTcpSocket> socket{ *m_socket };
                    queued_tcp_receiver<answer<T>> tcp{ socket };
                    tcp_transmit(question , *m_socket );
                    m_socket->ref().waitForBytesWritten();
                    if(!m_socket->ref().waitForReadyRead())
                        qDebug() << "Socket response could not be received";


                    auto queue = tcp.messages(speech::impl::identifier<answer<T>>{});

                    if(queue.empty())
                        qDebug() << "Message could not be retrieved";
                    else
                        qDebug() << "Answer received " << queue.front();

                    return queue.front();
                }

            private:

                std::unique_ptr<speech::handle::handle<QTcpSocket>> m_socket;

        };
    }
}

#endif