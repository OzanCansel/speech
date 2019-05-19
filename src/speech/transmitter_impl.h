#include "transmitter.h"
#include <QDataStream>
#include <functional>

namespace speech
{
namespace impl
{

template <typename H>
void pack_impl(QDataStream &)
{
}

template <typename H, typename... T>
void pack_impl(QDataStream &ss, const H &h, const T &... tail)
{
    ss << h;
    pack_impl(ss, tail...);
}

template <typename... T>
void pack_impl(const T &...) {}

template <typename... T>
QByteArray pack(const T &... values)
{
    QByteArray data;
    QDataStream ss(&data, QIODevice::WriteOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    pack_impl(ss, values...);

    return data;
}

template <size_t N, typename H, typename... T>
const QString transmitter_impl<N, H, T...>::type_hash = speech::impl::identify<H>();

template <size_t N, typename H, typename... T>
bool transmitter_impl<N, H, T...>::transmit(const H &value)
{

    auto data = pack(value);
    auto id_tagged = pack(type_hash, data);
    auto sized_pack = pack(241994, 1511999, 991973, id_tagged.size(), id_tagged);

    return write(sized_pack);
}

template <std::size_t i>
bool transmitter_impl<i>::transmit()
{
    throw std::bad_function_call();
}
} // namespace impl
} // namespace speech