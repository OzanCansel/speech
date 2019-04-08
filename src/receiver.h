#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include <QDataStream>
#include <tuple>
#include <vector>
#include <string>
#include <QString>
#include "concepts_utility.h"
#include "uniqueness.h"

template <size_t N, typename... T>
class ReceiverImpl;

template<size_t N>
struct Specializer { };

template<size_t N, typename Values>
void depackImpl(QDataStream& , Values& , Specializer<N>);

template<size_t N, typename Values, typename H, typename... T>
void depackImpl(QDataStream& , Values& , Specializer<N>);

template<typename... T>
std::tuple<T...> depack(const QByteArray&);

// Base case: empty tuple
template<std::size_t i>
class ReceiverImpl<i>
{
public:

    void receive(QString code, const QByteArray& data , Specializer<i> specializer);

};

template <size_t N, typename H, typename... T>
class ReceiverImpl <N, H, T...> :
        public ReceiverImpl <N + 1, T...>
{
public:

    static_assert(implements_right_stream<QDataStream , H>::value, "T must be deserialized from QDataStream, so must implement QDataStream& operator>>");
    static_assert(std::is_constructible<H>::value , "T must have default constructor.");

    constexpr static size_t SizeOfLengthTag = 8;
    static const QString typeHash;

    virtual ~ReceiverImpl() { }

    using ReceiverImpl <N + 1, T...>::receive;

    void receive(QString code, const QByteArray& data , Specializer<N> specializer);

protected:

    virtual void onEntityReceived(const H&);
    size_t parse(const QByteArray&);

};

template<typename... T>
using Receiver = ReceiverImpl<0 , T...>;


#endif // RECEIVER_H
