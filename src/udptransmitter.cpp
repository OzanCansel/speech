#include "udptransmitter.h"
#include <QUdpSocket>

template<typename... T>
UdpTransmitter<T...>::UdpTransmitter(QHostAddress addr, int port)
    :
      mAddr{ addr } ,
      mPort{ port }
{   }

template<typename... T>
int UdpTransmitter<T...>::port() const
{
    return mPort;
}

template<typename... T>
QHostAddress UdpTransmitter<T...>::destination() const
{
    return mAddr;
}

template<typename... T>
bool UdpTransmitter<T...>::write(const QByteArray & data)
{
    QUdpSocket socket;

    socket.writeDatagram(data , mAddr , mPort);
    socket.waitForBytesWritten();

    return true;
}

#include "transmitter.cpp"
