#ifndef SPEECH_IS_UNIQUE_PTR_H
#define SPEECH_IS_UNIQUE_PTR_H

#include <type_traits>
#include <memory>

template <class T>
struct is_unique_ptr : std::false_type
{};

template <class T , class D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type
{};

template <typename T , typename E>
struct is_unique_ptr_with : std::false_type
{};

template <typename T , typename D , typename E>
struct is_unique_ptr_with<std::unique_ptr< T , D> , E>
{
    static inline constexpr bool value = std::is_same_v< T , E >;
};

template<typename... T>
inline constexpr bool is_unique_ptr_v = is_unique_ptr<T...>::value;

template<typename... T>
inline constexpr bool is_unique_ptr_with_v = is_unique_ptr_with<T...>::value;

#endif
