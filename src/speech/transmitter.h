#ifndef SPEECH_TRANSMITTER_H
#define SPEECH_TRANSMITTER_H

#include <QDataStream>
#include <QByteArray>
#include <QString>
#include "util.h"

namespace speech
{

namespace impl
{

template <typename... T>
void pack_impl(const T &...);

template <typename H, typename... T>
void pack_impl(QDataStream &ss, const H &h, const T &... tail);

template <typename H>
void pack_impl(QDataStream &ss);

template <typename... T>
QByteArray pack(const T &... values);

template <size_t N, typename... T>
class transmitter_impl;

// Base case: empty tuple
template <std::size_t i>
class transmitter_impl<i>
{
  public:
    bool transmit();
};

template <size_t N, typename H, typename... T>
class transmitter_impl<N, H, T...> : public transmitter_impl<N + 1, T...>
{
  public:
    static_assert(implements_left_stream<QDataStream, H>::value, "T must be serialized to QDataStream, so must implement QDataStream& operator>>");
    static const QString type_hash;

    using transmitter_impl<N + 1, T...>::transmit;

    bool transmit(const H &value);
    virtual bool write(const QByteArray &) { return false; }
};
} // namespace impl

template <typename... T>
using transmitter = impl::transmitter_impl<0, T...>;

} // namespace speech

#include "transmitter_impl.h"

#endif