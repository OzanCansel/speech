#include "udpreceiver.h"

template<typename... T>
UdpReceiver<T...>::UdpReceiver(int port)
    :
      UdpReceiver<T...> { mBuiltInSocket , port }
{   }

template<typename... T>
UdpReceiver<T...>::UdpReceiver(QUdpSocket &socket)
    :
      mSocket(socket)
{   }

template<typename... T>
UdpReceiver<T...>::UdpReceiver(QUdpSocket &socket, int port)
    :
      mSocket{ socket } ,
      mPort{ port }
{
    if(!mSocket.bind(mPort))
    {
        QLOG_ERROR() << "Receiver socket could not bind to " << mPort;

        throw std::runtime_error("Socket could not bind");
    }

    QObject::connect(&mSocket , &QUdpSocket::readyRead , [this](){
        onDataReceived();
    });
}

template<typename... T>
void UdpReceiver<T...>::onDataReceived()
{
    while(mSocket.hasPendingDatagrams())
    {
        QByteArray datagram;

        datagram.resize(mSocket.pendingDatagramSize());

        //Read datagram
        QHostAddress clientAddress;
        quint16 clientPort;
        mSocket.readDatagram(datagram.data() , datagram.size() , &clientAddress , &clientPort);

        mBuffer.append(datagram);

        auto numberOfBytesProcessed = this->parse(mBuffer);

        //Clear processed bytes from buffer
        mBuffer.remove(0 , static_cast<int>(numberOfBytesProcessed) );
    }
}

#include "receiver.cpp"
