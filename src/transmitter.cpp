#include "transmitter.h"
#include <QDataStream>
#include <QDebug>
#include <string>
#include <functional>
#include "typehash.h"

template<typename H>
void packImpl(QDataStream&)
{   }

template<typename H, typename... T>
void packImpl(QDataStream& ss, const H& h, const T&... tail)
{
    ss << h;
    packImpl(ss , tail...);
}

template<typename... T>
void packImpl(const T&...)  {   }

template<typename... T>
QByteArray pack(const T&... values)
{
    QByteArray data;
    QDataStream ss(&data , QIODevice::WriteOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    packImpl(ss , values...);

    return data;
}

template <size_t N, typename H, typename... T>
QString TransmitterImpl <N, H, T...>::typeHash = TypeHash::unify(typeid(H).name());

template<size_t N, typename H, typename... T>
bool TransmitterImpl<N, H, T...>::transmit(const H& value)
{

    auto data = pack(value);
    auto idTagged = pack(typeHash , data);
    auto sizedPack = pack(idTagged.size() , idTagged);

    return write(sizedPack);
}

template<std::size_t i>
bool TransmitterImpl<i>::transmit()
{
    throw std::bad_function_call();
}


