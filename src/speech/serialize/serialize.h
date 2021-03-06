#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <QDataStream>
#include <QDebug>
#include <QStringList>
#include <QString>
#include "speech/util.h"
#include "speech/traits/implements_left_stream.h"

namespace speech
{
namespace impl
{

template<typename T>
void serialize_out( QDataStream& out , const T& head )
{
    out << head;
}

template<typename H , typename... T>
void serialize_out( QDataStream& out ,  const H& head , const T&... tail)
{
    out << head;
    serialize_out( out , tail... );
}

template<typename T>
void serialize_in( QDataStream& in , T& head )
{
    in >> head;
}

template<typename H , typename... T>
void serialize_in( QDataStream& in ,  H& head , T&... tail)
{
    in >> head;
    serialize_in( in , tail... );
}

template<int Idx , typename T>
void qdbg_out( const QStringList& field_names , QDebug& out , const T& head )
{
    out << field_names.at(Idx) << ":" << head << "}";
}

template<int Idx, typename H , typename... T>
void qdbg_out( const QStringList& field_names , QDebug& out ,  const H& head , const T&... tail)
{
    out << field_names.at(Idx) << ":" << head << ",";
    qdbg_out< Idx + 1 , T...>( field_names , out , tail... );
}

struct tag_speech_serialize{}; //It specifies that it is speech class and implements right and left stream operator
struct tag_speech_qdebug_out{};

}
}

#define SPEECH_SERIALIZE_EMPTY \
    public : \
    using __serialize_tag = speech::impl::tag_speech_serialize; \
    using __qdebug_tag = speech::impl::tag_speech_qdebug_out; \
    void operator <<( QDataStream& out ) const { } \
    void operator >>( QDataStream& in ) { } \
    void operator <<( QDebug& out ) const { } \


#define SPEECH_SERIALIZE(...) \
    SPEECH_JUST_SERIALIZE( __VA_ARGS__ ) \
    SPEECH_QDEBUG_OUT( __VA_ARGS__ )

#define SPEECH_JUST_SERIALIZE(...)\
    public : \
    using __serialize_tag = speech::impl::tag_speech_serialize; \
    void operator <<( QDataStream& out ) const \
    { \
        speech::impl::serialize_out( out , __VA_ARGS__ ) ;\
    } \
    void operator >>( QDataStream& in ) \
    { \
        speech::impl::serialize_in( in , __VA_ARGS__ ) ;\
    } \

#define SPEECH_QDEBUG_OUT(...) \
    public : \
    using __qdebug_tag = speech::impl::tag_speech_qdebug_out; \
    void operator <<( QDebug& out ) const \
    { \
        static const QStringList args_names = QString( #__VA_ARGS__ ).split( "," );\
        speech::impl::qdbg_out<0>( args_names , out , __VA_ARGS__ );\
    } \

template<typename Serializable , typename = typename std::is_same< typename Serializable::__serialize_tag , speech::impl::tag_speech_serialize>::type >
QDataStream& operator <<( QDataStream& out , const Serializable& e)
{
    e.operator<<( out );
    return out;
}

template<typename Serializable , typename = typename std::is_same< typename Serializable::__serialize_tag , speech::impl::tag_speech_serialize>::type >
QDataStream& operator >>( QDataStream& in , Serializable& e)
{
    e.operator >>( in );
    return in;
}

template<typename Serializable , typename = typename std::is_same< typename Serializable::__qdebug_tag , speech::impl::tag_speech_qdebug_out>::type >
QDebug operator <<( QDebug out , const Serializable& e)
{
    out.noquote() << speech::impl::identify<Serializable>() << "{";
    e.operator <<( out );
    return out;
}

template< typename Enum>
typename std::enable_if_t<std::is_enum<Enum>::value , QDataStream&>
operator <<( QDataStream& out , const Enum& e )
{
    int val = static_cast<int>(e);
    return out << val;
}

template< typename Enum>
typename std::enable_if_t<std::is_enum<Enum>::value , QDataStream&>
operator >>( QDataStream& in , Enum& e)
{
    int val = 0;
    in >> val;
    e = static_cast<Enum>( val );

    return in;
}

template < typename Enum>
typename std::enable_if_t<std::is_enum<Enum>::value, QDebug>
operator <<( QDebug out , const Enum& e )
{
    int val = static_cast<int>( e );
    return out << val;
}

#endif // SERIALIZE_H
