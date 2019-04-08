#include "askfor.h"
#include <QTime>
#include <QTcpSocket>
#include <QThread>
#include <QCoreApplication>
#include <QTime>
#include <QEventLoop>

template<typename S, typename R>
R AskFor<S , R>::operator()(const S& question)
{

    mReceived = false;

    QTime startTime;
    startTime.start();

    auto timeoutInMs = mTimeout.get() * 1000;

    TcpTransmitter<S>::transmit(question);

    if(!TcpTransmitter<S>::socket().waitForReadyRead(timeoutInMs))
    {
        throw timeout_exception { typeid (AskFor<S , R>).name() };
    }

    while(!mReceived)
    {
        QThread::msleep(1);

        if(startTime.elapsed() > timeoutInMs)
            throw timeout_exception { typeid (AskFor<S , R>).name() };
    }

    return std::move(mResponse);
}

template<typename S , typename R>
AskFor<S , R>::AskFor(QTcpSocket& socket , Timeout timeout)
    :
      TcpTransmitter<S> { socket } ,
      TcpReceiver<R> { socket } ,
      mTimeout { timeout }
{   }

template<typename S, typename R>
AskFor<S , R>::AskFor(QHostAddress addr, int port , Timeout timeout)
    :
      TcpTransmitter<S>{ addr , port },
      TcpReceiver<R> ( Transmitter::socket() ) ,
      mTimeout { timeout }
{   }

template<typename S, typename R>
AskFor<S , R>::AskFor(QTcpSocket &socket, QHostAddress addr, int port , Timeout timeout)
    :
      TcpTransmitter<S> { socket , addr , port } ,
      TcpReceiver<R> { socket } ,
      mTimeout { timeout }
{   }

template<typename S, typename R>
void AskFor<S , R>::onEntityReceived(const R& response)
{
    mResponse = response;
    mReceived = true;
}

#include "tcptransmitter.cpp"
#include "tcpreceiver.cpp"
