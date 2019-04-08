#include "receiver.h"
#include <iostream>
#include <exception>
#include <functional>
#include <algorithm>
#include <QDebug>
#include <QDataStream>
#include <regex>
#include "typehash.h"

template<size_t N, typename H, typename... T>
void ReceiverImpl<N, H, T...>::receive(QString code, const QByteArray& value , Specializer<N>)
{

    if(code != typeHash)
    {
        Specializer<N+1> s;
        return ReceiverImpl<N, H, T...>::receive(code , value , s);
    }

    QDataStream ss(&const_cast<QByteArray&>(value), QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);
    H val;

    ss >> val;

    onEntityReceived(val);
}

template<std::size_t i>
void ReceiverImpl<i>::receive(QString, const QByteArray& , Specializer<i>)
{   }

template <size_t N, typename H, typename... T>
//const size_t ReceiverImpl <N, H, T...>::typeHash = std::hash<std::string>{}(typeid(H).name());
const QString ReceiverImpl <N, H, T...>::typeHash = TypeHash::unify(typeid(H).name());

template <size_t N, typename H, typename... T>
size_t ReceiverImpl<N, H, T...>::parse(const QByteArray& entityData)
{
    //Check whether data is available
    if(entityData.isEmpty())
        return 0;

    QDataStream ss(&const_cast<QByteArray&>(entityData) , QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    QByteArray idTaggedArray;
    int size { };
    QString code { };
    QByteArray actualData;

    std::tie(size , idTaggedArray) = depack<int , QByteArray>(entityData);
    std::tie(code , actualData) = depack<QString , QByteArray>(idTaggedArray);

    auto totalRequiredLength = static_cast<size_t>(size) + static_cast<size_t>(SizeOfLengthTag);

    //If available bytes are not enough to express the entity
    if(totalRequiredLength < static_cast<size_t>(entityData.size()))
        return 0; //Return without parsing

     this->receive(code , actualData , Specializer<0>{});

    return totalRequiredLength;
}

template<size_t N, typename H, typename... T>
void ReceiverImpl<N, H, T...>::onEntityReceived(const H&)
{   }

template<typename... T>
std::tuple<T...> depack(const QByteArray & data)
{
    std::tuple<T...> values;
    QDataStream ss(&const_cast<QByteArray&>(data) , QIODevice::ReadOnly);
    ss.setVersion(QDataStream::Qt_5_0);

    depackImpl<0 , std::tuple<T...>, T...>(ss , values , Specializer<0>{ });

    return values;
}

template<size_t N, typename Values>
void depackImpl(QDataStream& , Values& , Specializer<N>)
{   }

template<size_t N, typename Values, typename H, typename... T>
void depackImpl(QDataStream& ss , Values& values, Specializer<N>)
{
    H h;
    ss >> h;
    std::get<N>(values) = std::move(h);

    depackImpl<N + 1 , Values , T...>(ss , values , Specializer<N + 1>{ });
}
