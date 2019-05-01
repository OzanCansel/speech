#ifndef SPEECH_UDP_RECEIVER_H
#define SPEECH_UDP_RECEIVER_H

#include "speech/receiver.h"
#include "speech/util.h"
#include <QUdpSocket>

namespace speech
{
namespace udp
{

template <typename... T>
class udp_receiver : public receiver<T...>
{
  public:

    udp_receiver(port);
    udp_receiver(QUdpSocket &);
    udp_receiver(QUdpSocket &, port);

  private:
    void on_data_received();

    QUdpSocket &m_socket;
    QUdpSocket m_built_in_socket;
    int m_port{};
    QByteArray m_buffer;
};
} // namespace udp
} // namespace speech

#include "udp_receiver_impl.h"

#endif