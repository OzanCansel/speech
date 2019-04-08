#ifndef TCPLINK_H
#define TCPLINK_H

#include "tcptransmitter.h"
#include "tcpreceiver.h"
#include "namedtype.h"
#include <exception>

using Timeout = NamedType<int , struct AskTimeoutParameter>;

struct timeout_exception : public std::runtime_error
{
    timeout_exception(std::string type) : std::runtime_error {  "Could not retrieved '" + type + "', operation failed." }{ }
};

template<typename S , typename R>
class AskFor : protected TcpTransmitter<S> , protected TcpReceiver<R>
{
public:

    using Transmitter = TcpTransmitter<S>;
    using Receiver = TcpReceiver<R>;

    R operator()(const S& = S{});

    AskFor(QTcpSocket& , Timeout = Timeout(10000));
    AskFor(QHostAddress, int , Timeout = Timeout(10000));
    AskFor(QTcpSocket& , QHostAddress , int , Timeout = Timeout(10000));


protected:

    void onEntityReceived(const R&) override;

private:

    Timeout mTimeout;
    R mResponse;
    bool mReceived { false };

};

#endif // TCPLINK_H
