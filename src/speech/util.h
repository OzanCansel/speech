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
     auto class_name = abi::__cxa_demangle ( typeid ( T ).name(), nullptr , nullptr , &status );

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

} // namespace impl


struct socket_deleter {
     void operator() ( QObject* obj ) const
     {
          obj->deleteLater();
     }
};

using port = impl::named_type<int, struct port_parameter>;

} // namespace speech

#endif // CONCEPTS_UTILITY_H
