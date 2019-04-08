#ifndef TCPTRANSMITTER_H
#define TCPTRANSMITTER_H

#include "transmitter.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <memory>

template<typename... T>
class TcpTransmitter : public Transmitter<T...>
{

public:

    TcpTransmitter(QHostAddress, int);
    TcpTransmitter(QTcpSocket&);
    TcpTransmitter(QTcpSocket& , QHostAddress , int);


    bool connectToHost(QHostAddress , int);

    QTcpSocket& socket();

protected:

    bool write(const QByteArray&) override;

private:

    QTcpSocket& mSocket;
    QTcpSocket mBuiltInSocket;

};

#endif // TCPTRANSMITTER_H
