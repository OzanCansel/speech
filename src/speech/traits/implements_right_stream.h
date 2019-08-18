#ifndef IMPLEMENTS_RIGHT_STREAM_H
#define IMPLEMENTS_RIGHT_STREAM_H

#include <type_traits>

namespace speech
{

template <typename S, typename T>
class implements_right_stream
{
    template <typename SS, typename TT>
    static auto test(int)
        -> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());

    template <typename, typename>
    static auto test(...) -> std::false_type;

  public:
    static const bool value = decltype(test<S, T>(0))::value;
};

}

#endif // IMPLEMENTS_RIGHT_STREAM_H
