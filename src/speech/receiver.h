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

template<bool enable , typename T>
struct message_queue
{ };

template<typename T>
struct message_queue<true , T>
{
  std::queue<T> m_messages;
  std::queue<T*> m_qobj_messages;
};

template <size_t N, bool Enable_Queue, typename... T>
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
template <std::size_t i , bool Enable_Queue>
class receiver_impl<i , Enable_Queue>
{
  public:
    virtual ~receiver_impl() { }
    void receive(QString code, const QByteArray &data, specializer<i> specializer);

    template<class T>
    std::queue<T>& messages(identifier<T>);
};

template <size_t N, bool Enable_Queue, typename H, typename... T>
class receiver_impl<N, Enable_Queue, H, T...> : 
                public receiver_impl<N + 1, Enable_Queue, T...> ,
                private message_queue<Enable_Queue , H>
{
  public:

    static_assert(implements_right_stream<QDataStream, H>::value, "T must be deserialized from QDataStream, so must implement QDataStream& operator>>");
    static_assert(std::is_constructible<H>::value, "T must have default constructor.");
    static_assert(std::is_base_of<QObject , H>::value || (std::is_copy_constructible<H>::value || std::is_move_constructible<H>::value) , "T type must be either copy constructible or move constructible.");

    constexpr static size_t SizeOfLengthTag = 8;
    static const QString type_hash;

    virtual ~receiver_impl() {}

    using receiver_impl<N + 1, Enable_Queue, T...>::receive;
    using receiver_impl<N + 1, Enable_Queue, T...>::messages;

    //Primary template
    template<typename Regular = H>
    typename std::enable_if<!std::is_base_of<QObject , Regular>::value>::type receive(QString code, const QByteArray &value, specializer<N> s);

    //Specialization for QObject
    template<typename QObj = H>
    typename std::enable_if<std::is_base_of<QObject , QObj>::value>::type receive(QString code, const QByteArray &value, specializer<N> s);

    // template<typename M>
    template <typename X>
    typename std::enable_if<std::is_same<X, H>::value && !std::is_base_of<X , QObject>::value, std::queue<X>&>::type messages(identifier<X> = identifier<X>{});

    template <typename X>
    typename std::enable_if<std::is_same<X, H>::value && std::is_base_of<X , QObject>::value, std::queue<X*>&>::type messages(identifier<X> = identifier<X>{});

  protected:

    virtual void on_receive(const H &);
    size_t parse(const QByteArray &);
    
  private:

  //Enqueue by moving
  template<typename Movable>
  typename std::enable_if<Enable_Queue && std::is_same<Movable , H>::value && std::is_move_constructible<Movable>::value>::type enqueue(H& val, identifier<H> = identifier<H>{});

  //Enqueue by copying
  template<typename NotMovable>
  typename std::enable_if<Enable_Queue && std::is_same<NotMovable , H>::value && !std::is_move_constructible<NotMovable>::value>::type enqueue(H& val, identifier<H> = identifier<H>{});

  //Does nothing, because messages are not queued.
  template<typename Entity>
  typename std::enable_if<!Enable_Queue && std::is_same<Entity , H>::value>::type enqueue(H& val, identifier<H> = identifier<H>{});

  std::queue<H*> m_qobj_messages;
  std::queue<H> m_messages;

};

} // namespace impl

template<typename... T>
using queued_receiver = impl::receiver_impl<0, true, T...>;

template <typename... T>
using receiver = impl::receiver_impl<0, false, T...>;

} // namespace speech

#include "receiver_impl.h"

#endif // RECEIVER_H
