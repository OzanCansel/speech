#ifndef CONCEPTS_UTILITY_H
#define CONCEPTS_UTILITY_H

#include <type_traits>

template<typename S, typename T>
class implements_left_stream
{
    template<typename SS, typename TT>
    static auto test(int)
    -> decltype( std::declval<SS&>() << std::declval<TT>(), std::true_type() );

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S,T>(0))::value;
};

template<typename S, typename T>
class implements_right_stream
{
    template<typename SS, typename TT>
    static auto test(int)
    -> decltype( std::declval<SS&>() << std::declval<TT>(), std::true_type() );

    template<typename, typename>
    static auto test(...) -> std::false_type;

public:
    static const bool value = decltype(test<S,T>(0))::value;
};

#endif // CONCEPTS_UTILITY_H
