#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include "receiver.h"
#include <QUdpSocket>

template<typename... T>
class UdpReceiver : public Receiver<T...>
{
public:

    UdpReceiver(int port);
    UdpReceiver(QUdpSocket&);
    UdpReceiver(QUdpSocket& , int);

private:

    void onDataReceived();

    QUdpSocket& mSocket;
    QUdpSocket mBuiltInSocket;
    int mPort{ };
    QByteArray mBuffer;

};

#endif // UDPRECEIVER_H
