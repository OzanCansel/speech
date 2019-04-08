#include "tcpreceiver.h"
#include <memory>

template<typename... T>
TcpReceiver<T...>::TcpReceiver(QHostAddress host, int port)
    :
      TcpReceiver<T...>{ mBuiltInSocket , host , port }
{   }

template<typename... T>
TcpReceiver<T...>::TcpReceiver(QTcpSocket& socket)
    :
      mSocket{ socket }
{
    listen();
}

template<typename... T>
TcpReceiver<T...>::TcpReceiver(QTcpSocket& socket, QHostAddress host, int port)
    :
      mSocket { socket } ,
      mHost { host } ,
      mPort{ port }
{
    listen();
}

template<typename... T>
QTcpSocket& TcpReceiver<T...>::socket()
{
    return mSocket;
}

template<typename... T>
void TcpReceiver<T...>::onDataReceived()
{
    while(mSocket.bytesAvailable())
    {
        mBuffer.append(mSocket.readAll());

        auto numberOfBytesProcessed = this->parse(mBuffer);

        //Clear processed bytes from buffer
        mBuffer.remove(0 , static_cast<int>(numberOfBytesProcessed));
    }
}

template<typename... T>
void TcpReceiver<T...>::listen()
{
    QObject::connect(&mSocket , &QTcpSocket::readyRead , [this](){
        onDataReceived();
    });
}

#include "receiver.cpp"
