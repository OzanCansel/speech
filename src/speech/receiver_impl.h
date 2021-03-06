#include "receiver.h"
#include <QDataStream>
#include <exception>

namespace speech
{
namespace impl
{

template<typename T>
void message_queue<false, T>::
enqueue ( T& val, identifier<T> )
{   }

template<typename T>
template<typename Movable>
typename std::enable_if<std::is_move_constructible<Movable>::value>::type message_queue<true, T, typename std::enable_if<!std::is_base_of<QObject, T>::value>::type>::
enqueue ( Movable& val, identifier<Movable> )
{
    m_messages.push ( std::move ( val ) );
}

template<typename T>
template<typename NotMovable>
typename std::enable_if<!std::is_move_constructible<NotMovable>::value>::type message_queue<true, T, typename std::enable_if<!std::is_base_of<QObject, T>::value>::type>::
enqueue ( NotMovable& val, identifier<NotMovable> )
{
    m_messages.push ( val );
}

template <std::size_t i, bool EnableQueue>
bool receiver_impl<i, EnableQueue>::
receive ( const QString& , const QByteArray &, specializer<i> )
{
    return false;
}

template<typename T>
std::queue<T*>& message_queue<true, T,  typename std::enable_if<std::is_base_of<QObject, T>::value>::type>::
messages ( identifier<T> )
{
    return m_messages;
}

template<typename T>
std::queue<T>& message_queue<true, T, typename std::enable_if<!std::is_base_of<QObject, T>::value>::type>::
messages ( identifier<T> )
{
    return m_messages;
}


template <size_t N, bool EnableQueue, typename H, typename... T>
const QString receiver_impl<N, EnableQueue, H, T...>::type_hash = speech::impl::identify<H>();

template <size_t N, bool EnableQueue, typename H, typename... T>
template <typename Regular>
typename std::enable_if<!std::is_base_of<QObject, Regular>::value, bool>::type receiver_impl<N, EnableQueue, H, T...>::
receive ( const QString& code, const QByteArray &value, specializer<N> )
{
    if ( code != type_hash ) {
        specializer<N + 1> s;
        return receiver_impl<N, EnableQueue, Regular, T...>::receive ( code, value, s );
    }

    QDataStream ss { value };
    ss.setVersion ( QDataStream::Qt_5_0 );
    Regular val;

    ss >> val;

    on_receive ( val );

    message_queue<EnableQueue, H>::enqueue ( val );

    return true;
}

template <size_t N, bool EnableQueue, typename H, typename... T>
template <typename QObj>
typename std::enable_if<std::is_base_of<QObject, QObj>::value, bool>::type receiver_impl<N, EnableQueue, H, T...>::
receive ( const QString& code, const QByteArray &value, specializer<N> )
{
    if ( code != type_hash ) {
        specializer<N + 1> s;
        return receiver_impl<N, EnableQueue, QObj, T...>::receive ( code, value, s );
    }

    QDataStream ss { value };
    ss.setVersion ( QDataStream::Qt_5_0 );
    auto val = new QObj();

    ss >> *val;

    message_queue<EnableQueue, H>::enqueue ( *val );

    on_receive ( *val );

    return true;
}

template <size_t N, bool EnableQueue, typename H, typename... T>
int receiver_impl<N, EnableQueue, H, T...>::
parse ( const QByteArray &entity_data )
{
    //Check whether data is available
    if ( entity_data.isEmpty() ) {
        return 0;
    }

    QDataStream ss { entity_data };
    ss.setVersion ( QDataStream::Qt_5_0 );

    QByteArray id_tagged_array;
    int token1{}, token2{}, token3{};
    int size{};
    quint16 crc{};
    QString code;
    QByteArray actual_data;

    std::tie ( token1, token2, token3, crc , size, id_tagged_array ) = depack<int, int, int, quint16 , int, QByteArray> ( entity_data );

    //They are just constant, hard-coded, mystified numbers
    if ( token1 != 241994 ||
         token2 != 1511999 ||
         token3 != 991973 ) {
        on_token_error ( entity_data );
        on_error ( entity_data );
        return -2;
    }

    // Do checksum for data
    if ( qChecksum( id_tagged_array.data() , static_cast<uint>( id_tagged_array.size() ) ) != crc )
    {
        on_checksum_error( entity_data );
        on_error( entity_data );
        return -2;
    }

    std::tie ( code, actual_data ) = depack<QString, QByteArray> ( id_tagged_array );

    //                            sizeof tokens
    auto total_required_length = 3 * sizeof ( int ) + sizeof( quint16 ) + SizeOfLengthTag + size;

    //If available bytes are not enough to express the entity
    if ( total_required_length > static_cast<size_t> ( entity_data.size() ) ) {
        return 0;     //There are remaining parts
    }

    bool parsed = receive ( code, actual_data, specializer<0> {} );

    return parsed ? total_required_length : -1;
}

template <size_t N, bool EnableQueue, typename H, typename... T>
void receiver_impl<N, EnableQueue, H, T...>::
on_receive ( const H & )
{
}

template <typename... T>
std::tuple<T...> depack ( const QByteArray &data )
{
    std::tuple<T...> values;
    QDataStream ss { data };
    ss.setVersion ( QDataStream::Qt_5_0 );

    depack_impl<0, std::tuple<T...>, T...> ( ss, values, specializer<0> {} );

    return values;
}

template <size_t N, typename Values>
void depack_impl ( QDataStream &, Values &, specializer<N> )
{
}

template <size_t N, typename Values, typename H, typename... T>
void depack_impl ( QDataStream &ss, Values &values, specializer<N> )
{
    H h;
    ss >> h;
    std::get<N> ( values ) = std::move ( h );

    depack_impl<N + 1, Values, T...> ( ss, values, specializer<N + 1> {} );
}

} // namespace impl
} // namespace speech
