#ifndef SPEECH_RECEIVER_H
#define SPEECH_RECEIVER_H

#include <QDataStream>
#include <QString>
#include <QObject>
#include <tuple>
#include <queue>
#include "util.h"
#include "speech/traits/implements_right_stream.h"

namespace speech
{

namespace impl
{

template <size_t N,  bool EnableQueue, typename... T>
class receiver_impl;

template <size_t N>
struct specializer {};

template<class I>
struct identifier {};

template <size_t N, typename Values>
void depack_impl ( QDataStream &, Values &, specializer<N> );

template <size_t N, typename Values, typename H, typename... T>
void depack_impl ( QDataStream &, Values &, specializer<N> );

template <typename... T>
std::tuple<T...> depack ( const QByteArray & );

template<bool EnableQueue, typename T, typename = void>
class message_queue;

template<typename T>
class message_queue<false, T>
{
public:
    inline std::queue<T*>& messages ( identifier<T> = identifier<T> {} ) = delete;
    void enqueue ( T& val, identifier<T> = identifier<T> {} );
};

//If  it is QObject
template<typename T>
class message_queue<true, T, typename std::enable_if<std::is_base_of<QObject, T>::value>::type>
{
public:
    inline std::queue<T*>& messages ( identifier<T> = identifier<T> {} );
protected:
    std::queue<T*> m_messages;
};

//If it is regular
template<typename T>
class message_queue<true, T, typename std::enable_if<!std::is_base_of<QObject, T>::value>::type>
{
public:

    inline std::queue<T>& messages ( identifier<T> = identifier<T> {} );

    template<typename Movable = T>
    typename std::enable_if<std::is_move_constructible<Movable>::value>::type enqueue ( Movable& val, identifier<Movable> = identifier<Movable> {} );

    //Enqueue by copying
    template<typename NotMovable = T>
    typename std::enable_if<!std::is_move_constructible<NotMovable>::value>::type enqueue ( NotMovable& val, identifier<NotMovable> = identifier<NotMovable> {} );

protected:

    std::queue<T> m_messages;
};

template <std::size_t i, bool EnableQueue>
class receiver_impl<i, EnableQueue>
{
public:
    bool receive ( QString code, const QByteArray &data, specializer<i> specializer );

    void messages() = delete;

protected:

    virtual void on_checksum_error( const QByteArray& ) { }
    virtual void on_token_error( const QByteArray& ) { }
    virtual void on_error( const QByteArray& ) { }

};

template <size_t N,  bool EnableQueue, typename H, typename... T>
class receiver_impl<N, EnableQueue, H, T...> :
        public receiver_impl<N + 1, EnableQueue, T...>,
        public message_queue<EnableQueue, H>
{
public:

    static_assert ( implements_right_stream<QDataStream, H>::value, "T must be deserialized from QDataStream, so must implement QDataStream& operator>>" );
    static_assert ( std::is_constructible<H>::value, "T must have default constructor." );
    static_assert ( std::is_base_of<QObject, H>::value || ( std::is_copy_constructible<H>::value || std::is_move_constructible<H>::value ), "T type must be either copy constructible or move constructible." );

    constexpr static size_t SizeOfLengthTag = 8;
    static const QString type_hash;

    virtual ~receiver_impl() {}

    using receiver_impl<N + 1 , EnableQueue , T...>::receive;
    using receiver_impl<N + 1 , EnableQueue , T...>::messages;
    using receiver_impl<N + 1 , EnableQueue , T...>::on_checksum_error;
    using receiver_impl<N + 1 , EnableQueue , T...>::on_token_error;
    using receiver_impl<N + 1 , EnableQueue , T...>::on_error;

    template<typename E =  H, bool M = EnableQueue, typename = typename std::enable_if<M && std::is_same<E, H>::value>::type>
    decltype ( std::declval<message_queue<true, H>>().messages() ) messages ( identifier<H> = identifier<H> {} )
    {
        return message_queue<EnableQueue, H>::messages ( identifier<E> {} );
    }

    //Primary template
    template<typename Regular = H>
    typename std::enable_if<!std::is_base_of<QObject, Regular>::value, bool>::type
    receive ( QString code, const QByteArray &value, specializer<N> s );

    //Specialization for QObject
    template<typename QObj = H>
    typename std::enable_if<std::is_base_of<QObject, QObj>::value, bool>::type
    receive ( QString code, const QByteArray &value, specializer<N> s );

protected:

    virtual void on_receive ( const H & );
    int parse ( const QByteArray & );

};

} // namespace impl

template <bool EnableQueue, typename... T>
using receiver = impl::receiver_impl<0, EnableQueue, T...>;

} // namespace speech

#include "receiver_impl.h"

#endif // RECEIVER_H
