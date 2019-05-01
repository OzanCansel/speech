#ifndef SPEECH_RECEIVER_H
#define SPEECH_RECEIVER_H

#include <QDataStream>
#include <QString>
#include <QObject>
#include <tuple>
#include <queue>
#include "util.h"

namespace speech
{

namespace impl
{

template <size_t N, typename... T>
class receiver_impl;

template <size_t N>
struct specializer{};

template<class I>
struct identifier{};

template <size_t N, typename Values>
void depack_impl(QDataStream &, Values &, specializer<N>);

template <size_t N, typename Values, typename H, typename... T>
void depack_impl(QDataStream &, Values &, specializer<N>);

template <typename... T>
std::tuple<T...> depack(const QByteArray &);

// Base case: empty tuple
template <std::size_t i>
class receiver_impl<i>
{
  public:
    void receive(QString code, const QByteArray &data, specializer<i> specializer);

    template<class T>
    std::queue<T>& messages(identifier<T>);
};

template <size_t N, typename H, typename... T>
class receiver_impl<N, H, T...> : public receiver_impl<N + 1, T...>
{
  public:

    static_assert(implements_right_stream<QDataStream, H>::value, "T must be deserialized from QDataStream, so must implement QDataStream& operator>>");
    static_assert(std::is_constructible<H>::value, "T must have default constructor.");

    constexpr static size_t SizeOfLengthTag = 8;
    static const QString type_hash;

    virtual ~receiver_impl() {}

    using receiver_impl<N + 1, T...>::receive;
    using receiver_impl<N + 1, T...>::messages;

    //Primary template
    template<typename Regular = H>
    typename std::enable_if<!std::is_base_of<QObject , Regular>::value>::type receive(QString code, const QByteArray &value, specializer<N> s)
    { 
        if (code != type_hash)
        {
            specializer<N + 1> s;
            return receiver_impl<N, Regular, T...>::receive(code, value, s);
        }

        QDataStream ss(&const_cast<QByteArray &>(value), QIODevice::ReadOnly);
        ss.setVersion(QDataStream::Qt_5_0);
        Regular val;

        ss >> val;

        on_receive(val);

        m_messages.push(std::move(val));
    }

    //Specialization for QObject
    template<typename QObj = H>
    typename std::enable_if<std::is_base_of<QObject , QObj>::value>::type receive(QString code, const QByteArray &value, specializer<N> s) 
    {
        if (code != type_hash)
        {
            specializer<N + 1> s;
            return receiver_impl<N, QObj, T...>::receive(code, value, s);
        }

        QDataStream ss(&const_cast<QByteArray &>(value), QIODevice::ReadOnly);
        ss.setVersion(QDataStream::Qt_5_0);
        auto val = new QObj();

        ss >> *val;

        on_receive(*val);
    }

    // template<typename M>
    template <typename X>
    inline typename std::enable_if<std::is_same<X, H>::value, std::queue<H>&>::type messages(identifier<H> = identifier<H>{}) { return m_messages; }

  protected:

    virtual void on_receive(const H &);
    size_t parse(const QByteArray &);
    
  private:

    std::queue<H> m_messages;
};

} // namespace impl

template <typename... T>
using receiver = impl::receiver_impl<0, T...>;

} // namespace speech

#include "receiver_impl.h"

#endif // RECEIVER_H
