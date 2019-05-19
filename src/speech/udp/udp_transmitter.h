#ifndef SPEECH_UDP_TRANSMITTER_H
#define SPEECH_UDP_TRANSMITTER_H

#include <QHostAddress>
#include "speech/util.h"
#include "speech/transmitter.h"

namespace speech
{
namespace udp
{
template <typename... T>
class udp_transmitter : public transmitter<T...>
{

  public:

    udp_transmitter(QHostAddress, speech::port port);
    int port() const;
    QHostAddress destination() const;

  protected:
    bool write(const QByteArray &) override;

  private:
    QHostAddress m_addr{};
    int m_port{-1};

};
} // namespace udp
} // namespace speech

#include "udp_transmitter_impl.h"

#endif // UDPTRANSMITTER_H
