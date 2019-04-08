#ifndef TCPRECEIVER_H
#define TCPRECEIVER_H

#include <QTcpSocket>
#include <QHostAddress>
#include "receiver.h"

template<typename... T>
class TcpReceiver : protected Receiver<T...>
{

public:

    TcpReceiver(QHostAddress host, int port);
    TcpReceiver(QTcpSocket& socket);
    TcpReceiver(QTcpSocket& socket, QHostAddress, int);

protected:

    QTcpSocket& socket();

private:

    void onDataReceived();
    void listen();

    QTcpSocket& mSocket;
    QTcpSocket mBuiltInSocket;
    int mPort;
    QHostAddress mHost;
    QByteArray mBuffer;

};

#endif // TCPRECEIVER_H
