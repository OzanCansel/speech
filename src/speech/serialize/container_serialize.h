#ifndef VECTOR_SERIALIZE_H
#define VECTOR_SERIALIZE_H

#include <QDataStream>
#include <QDebug>
#include <type_traits>
#include "speech/traits/is_container.h"

namespace speech
{

namespace serialization
{

namespace container
{

template<typename Cont , typename = typename std::enable_if<is_container<Cont>::value>::type>
QDataStream& operator<<( QDataStream& out , const Cont& cont )
{
    out << static_cast<int>( cont.size() );

    for( const auto& el : cont )
        out << el;

    return out;
}

template<typename Cont , typename = typename std::enable_if<is_container<Cont>::value>::type>
QDataStream& operator>>( QDataStream& in , Cont& cont )
{
    int size{};
    in >> size;

    for(auto i = 0; i < size; ++i)
    {
        typename Cont::value_type el;
        in >> el;
        cont.push_back( el );
    }

    return in;
}

template<typename Cont , typename = typename std::enable_if<is_container<Cont>::value>::type>
QDebug operator<<( QDebug out , const Cont& cont)
{
    out << " [ ";

    for( auto i = 0; i < cont.size() - 1; ++i)
        out << cont.at(i) << " , ";

    return out << cont.at(cont.size() - 1) << " ] ";
}

}
}
}




#endif // VECTOR_SERIALIZE_H
