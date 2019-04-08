#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <iostream>
#include <exception>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>
#include <QString>
#include "concepts_utility.h"

template<typename... T>
void packImpl(const T&...);

template<typename H, typename... T>
void packImpl(QDataStream& ss, const H& h, const T&... tail);

template<typename H>
void packImpl(QDataStream& ss);

template<typename... T>
QByteArray pack(const T&... values);

template <size_t N, typename... T>
class TransmitterImpl;

// Base case: empty tuple
template<std::size_t i>
class TransmitterImpl<i>
{
public:
    bool transmit();
};

template <size_t N, typename H, typename... T>
class TransmitterImpl <N, H, T...> :
        public TransmitterImpl <N + 1, T...>
{
public:

    static_assert(implements_left_stream<QDataStream , H>::value, "T must be serialized to QDataStream, so must implement QDataStream& operator>>");
    static QString typeHash;

    using TransmitterImpl <N + 1, T...>::transmit;


    bool transmit(const H& value);
    virtual bool write(const QByteArray&) {  return false; }

};

template<typename... T>
using Transmitter = TransmitterImpl<0 , T...>;


#endif // GROUPTRANSMITTER_H
