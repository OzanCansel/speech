#include "tcptransmitter.h"
#include <exception>

template<typename... T>
TcpTransmitter<T...>::TcpTransmitter(QHostAddress host, int port)
    :
      TcpTransmitter<T...>{ mBuiltInSocket , host , port }
{   }

template<typename... T>
TcpTransmitter<T...>::TcpTransmitter(QTcpSocket & socket)
    :
      mSocket { socket }
{   }

template<typename... T>
TcpTransmitter<T...>::TcpTransmitter(QTcpSocket &socket, QHostAddress host, int port)
    :
      mSocket { socket }
{
    if(!mSocket.isOpen()){
        connectToHost(host , port);
    }
}

template<typename... T>
bool TcpTransmitter<T...>::connectToHost(QHostAddress host, int port)
{
    mSocket.connectToHost(host , port);

    if(!mSocket.waitForConnected())
    {
        QLOG_ERROR() << "Could not connect to " << host.toString() << ":" << port;
        return false;
    }

    QLOG_INFO() << "Connected to " << host.toString() << ":" << port;

    return true;
}

template<typename... T>
QTcpSocket &TcpTransmitter<T...>::socket()
{
    return mSocket;
}

template<typename... T>
bool TcpTransmitter<T...>::write(const QByteArray& data)
{

    if(!mSocket.isOpen())
        return false;

    mSocket.write(data);
    mSocket.waitForBytesWritten();

    return true;
}

#include "transmitter.cpp"
