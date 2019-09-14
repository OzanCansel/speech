#ifndef SPEECH_IS_SHARED_PTR_H
#define SPEECH_IS_SHARED_PTR_H

#include <type_traits>
#include <memory>

template<typename T>
struct is_shared_ptr : std::false_type
{};

template<typename T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type
{};

template<typename T, typename E>
struct is_shared_ptr_with : std::false_type
{};

template<typename T , typename E>
struct is_shared_ptr_with< std::shared_ptr<T> , E>
{
    static inline constexpr bool value = std::is_same_v< E , T >;
};

template<typename... T>
inline constexpr bool is_shared_ptr_v = is_shared_ptr<T...>::value;

template<typename... T>
inline constexpr bool is_shared_ptr_with_v = is_shared_ptr_with<T...>::value;

#endif
