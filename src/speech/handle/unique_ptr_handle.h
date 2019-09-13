#ifndef SPEECH_UNIQUE_PTR_HANDLE_H
#define SPEECH_UNIQUE_PTR_HANDLE_H

#include <memory>
#include "handle.h"

namespace speech
{
namespace handle
{
template<typename T , typename Deleter>
struct unique_ptr_handle : handle<T> {

     explicit unique_ptr_handle ( std::unique_ptr< T , Deleter > sck ) : handle<T>{ *sck.get() }, m_sck{ std::move ( sck ) }
     {   }

private:

     std::unique_ptr<T , Deleter> m_sck;
};
}
}

#endif
