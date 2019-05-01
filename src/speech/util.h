#ifndef SPEECH_UTIL_H
#define SPEECH_UTIL_H

#include <QString>
#include <type_traits>
#include <type_info>

namespace speech
{

namespace impl
{

template <typename S, typename T>
class implements_left_stream
{
    template <typename SS, typename TT>
    static auto test(int)
        -> decltype(std::declval<SS &>() << std::declval<TT>(), std::true_type());

    template <typename, typename>
    static auto test(...) -> std::false_type;

  public:
    static const bool value = decltype(test<S, T>(0))::value;
};

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


template<class T>
QString identify()
{
    return  QString::fromStdString(typeid(T).name());
}

template <typename T, typename param>
class named_type
{
public:
    explicit named_type(T const& value) : value_(value) {}
    explicit named_type(T&& value = T{}) : value_(std::move(value)) {}
    T& get() { return value_; }
    T const& get() const {return value_; }
private:
    T value_;
};

} // namespace impl

using port = impl::named_type<int , struct port_parameter>;

} // namespace speech

#endif // CONCEPTS_UTILITY_H
