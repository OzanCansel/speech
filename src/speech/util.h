#ifndef SPEECH_UTIL_H
#define SPEECH_UTIL_H

#include <QString>
#include <QObject>
#include <type_traits>
#include <typeinfo>
#include <regex>

namespace speech
{

namespace impl
{

#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER))
#define CLANG_COMPILER (defined(__clang__))

#if GCC_COMPILER || CLANG_COMPILER
#include <cxxabi.h>
#endif

template <typename S, typename T>
class implements_left_stream
{
     template <typename SS, typename TT>
     static auto test ( int )
     -> decltype ( std::declval<SS &>() << std::declval<TT>(), std::true_type() );

     template <typename, typename>
     static auto test ( ... ) -> std::false_type;

public:
     static const bool value = decltype ( test<S, T> ( 0 ) ) ::value;
};

template <typename S, typename T>
class implements_right_stream
{
     template <typename SS, typename TT>
     static auto test ( int )
     -> decltype ( std::declval<SS &>() << std::declval<TT>(), std::true_type() );

     template <typename, typename>
     static auto test ( ... ) -> std::false_type;

public:
     static const bool value = decltype ( test<S, T> ( 0 ) ) ::value;
};


template<class T>
QString identify()
{

#if defined(_MSC_BUILD)

     std::regex pat{ R"(^.+\s)" };
     auto unified_str = std::regex_replace ( typeid ( T ).name(), pat, "" );
     return QString::fromStdString ( unified_str );

#endif

#if GCC_COMPILER || CLANG_COMPILER
     int status;
     auto class_name = abi::__cxa_demangle ( typeid ( T ).name(), 0, 0, &status );

     return QString::fromLatin1 ( class_name );
#endif

     //Default identifier
     return  QString::fromStdString ( typeid ( T ).name() );
}

template <typename T, typename param>
class named_type
{
public:
     explicit named_type ( T const& value ) : m_value ( value ) {}
     explicit named_type ( T&& value = T{} ) : m_value ( std::move ( value ) ) {}
     T& get()
     {
          return m_value;
     }
     T const& get() const
     {
          return m_value;
     }
private:
     T m_value;
};

struct qobject_deleter {
     void operator() ( QObject* obj )
     {
          obj->deleteLater();
     }
};

} // namespace impl

using port = impl::named_type<int, struct port_parameter>;

} // namespace speech

#endif // CONCEPTS_UTILITY_H
