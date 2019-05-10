#include "receiver.h"
#include <QDataStream>
#include <exception>

namespace speech
{
namespace impl
{

template <std::size_t i , bool Enable_Queue>
void receiver_impl<i , Enable_Queue>::receive(QString, const QByteArray &, specializer<i>)
{
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
const QString receiver_impl<N, Enable_Queue, H, T...>::type_hash = speech::impl::identify<H>();

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename Regular>
typename std::enable_if<!std::is_base_of<QObject, Regular>::value>::type receiver_impl<N, Enable_Queue, H, T...>::receive(QString code, const QByteArray &value, specializer<N> s)
{
    if (code != type_hash)
    {
        specializer<N + 1> s;
        return receiver_impl<N, Enable_Queue, Regular, T...>::receive(code, value, s);
    }

    QDataStream ss(&const_cast<QByteArray &>(value), QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);
    Regular val;

    ss >> val;

    //Enqueue object by moving if it is not possible, copy then
    enqueue<Regular>(val);

    on_receive(m_messages.back());
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename QObj>
typename std::enable_if<std::is_base_of<QObject, QObj>::value>::type receiver_impl<N, Enable_Queue, H, T...>::receive(QString code, const QByteArray &value, specializer<N> s)
{
    if (code != type_hash)
    {
        specializer<N + 1> s;
        return receiver_impl<N, Enable_Queue, QObj, T...>::receive(code, value, s);
    }

    QDataStream ss(&const_cast<QByteArray &>(value), QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);
    auto val = new QObj();

    ss >> *val;

    m_qobj_messages.push(val);

    on_receive(*val);
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename Movable>
typename std::enable_if<Enable_Queue && std::is_same<Movable, H>::value && std::is_move_constructible<Movable>::value>::type
receiver_impl<N, Enable_Queue, H, T...>::enqueue(H &val, identifier<H>)
{
    m_messages.push(std::move(val));
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename NotMovable>
typename std::enable_if<Enable_Queue && std::is_same<NotMovable, H>::value && !std::is_move_constructible<NotMovable>::value>::type
receiver_impl<N, Enable_Queue, H, T...>::enqueue(H &val, identifier<H>)
{
    m_messages.push(val);
}

template<size_t N, bool Enable_Queue, typename H, typename... T>
template<typename Entity>
typename std::enable_if<!Enable_Queue && std::is_same<Entity , H>::value>::type  
receiver_impl<N, Enable_Queue, H, T...>::enqueue(H& val, identifier<H>)
{   }

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename X>
typename std::enable_if<std::is_same<X, H>::value && !std::is_base_of<X , QObject>::value, std::queue<X>&>::type receiver_impl<N, Enable_Queue, H, T...>::messages(identifier<X>)
{
    return m_messages;
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
template <typename X>
typename std::enable_if<std::is_same<X, H>::value && std::is_base_of<X , QObject>::value, std::queue<X*>&>::type receiver_impl<N, Enable_Queue, H, T...>::messages(identifier<X>)
{
    return m_qobj_messages;
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
size_t receiver_impl<N, Enable_Queue, H , T...>::parse(const QByteArray &entity_data)
{
    //Check whether data is available
    if (entity_data.isEmpty())
        return 0;

    QDataStream ss(&const_cast<QByteArray &>(entity_data), QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    QByteArray id_tagged_array;
    int size{};
    QString code{};
    QByteArray actual_data;

    std::tie(size, id_tagged_array) = depack<int, QByteArray>(entity_data);
    std::tie(code, actual_data) = depack<QString, QByteArray>(id_tagged_array);

    auto total_required_length = static_cast<size_t>(size) + static_cast<size_t>(SizeOfLengthTag);

    //If available bytes are not enough to express the entity
    if (total_required_length > static_cast<size_t>(entity_data.size()))
        return 0; //Return without parsing

    this->receive(code, actual_data, specializer<0>{});

    return total_required_length;
}

template <size_t N, bool Enable_Queue, typename H, typename... T>
void receiver_impl<N, Enable_Queue, H, T...>::on_receive(const H &)
{
}

template <typename... T>
std::tuple<T...> depack(const QByteArray &data)
{
    std::tuple<T...> values;
    QDataStream ss(&const_cast<QByteArray &>(data), QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    depack_impl<0, std::tuple<T...>, T...>(ss, values, specializer<0>{});

    return values;
}

template <size_t N, typename Values>
void depack_impl(QDataStream &, Values &, specializer<N>)
{
}

template <size_t N, typename Values, typename H, typename... T>
void depack_impl(QDataStream &ss, Values &values, specializer<N>)
{
    H h;
    ss >> h;
    std::get<N>(values) = std::move(h);

    depack_impl<N + 1, Values, T...>(ss, values, specializer<N + 1>{});
}

} // namespace impl
} // namespace speech