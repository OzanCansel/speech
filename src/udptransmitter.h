#ifndef UDPTRANSMITTER_H
#define UDPTRANSMITTER_H

#include "transmitter.h"
#include <QHostAddress>

template <typename... T>
class UdpTransmitter : public Transmitter<T...>
{

public:

    UdpTransmitter(QHostAddress mAddr , int port);
    int port() const;
    QHostAddress destination() const;

protected:

    bool write(const QByteArray&) override;

private:

    QHostAddress mAddr { };

    int mPort{ -1};

};

#endif // UDPTRANSMITTER_H
