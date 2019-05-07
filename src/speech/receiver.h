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
    static_assert(std::is_base_of<QObject , H>::value || (std::is_copy_constructible<H>::value || std::is_move_constructible<H>::value) , "T type must be either copy constructible or move constructible.");

    constexpr static size_t SizeOfLengthTag = 8;
    static const QString type_hash;

    virtual ~receiver_impl() {}

    using receiver_impl<N + 1, T...>::receive;
    using receiver_impl<N + 1, T...>::messages;

    //Primary template
    template<typename Regular = H>
    typename std::enable_if<!std::is_base_of<QObject , Regular>::value>::type receive(QString code, const QByteArray &value, specializer<N> s);

    //Specialization for QObject
    template<typename QObj = H>
    typename std::enable_if<std::is_base_of<QObject , QObj>::value>::type receive(QString code, const QByteArray &value, specializer<N> s);

    // template<typename M>
    template <typename X>
    typename std::enable_if<std::is_same<X, H>::value && !std::is_base_of<X , QObject>::value, std::queue<H>&>::type messages(identifier<H> = identifier<H>{});

    template <typename X>
    typename std::enable_if<std::is_same<X, H>::value && std::is_base_of<X , QObject>::value, std::queue<H*>&>::type messages(identifier<H> = identifier<H>{});

  protected:

    virtual void on_receive(const H &);
    size_t parse(const QByteArray &);
    
  private:

  //Enqueue by moving
  template<typename Movable>
  typename std::enable_if<std::is_same<Movable , H>::value && std::is_move_constructible<Movable>::value>::type enqueue(H& val, identifier<H> = identifier<H>{});

  //Enqueue by copying
  template<typename NotMovable>
  typename std::enable_if<std::is_same<NotMovable , H>::value && !std::is_move_constructible<NotMovable>::value>::type enqueue(H& val, identifier<H> = identifier<H>{});

  std::queue<H*> m_qobj_messages;
  std::queue<H> m_messages;
};

} // namespace impl

template <typename... T>
using receiver = impl::receiver_impl<0, T...>;

} // namespace speech

#include "receiver_impl.h"

#endif // RECEIVER_H
