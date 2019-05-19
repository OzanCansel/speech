#ifndef SPEECH_UNIQUE_PTR_HANDLE_H
#define SPEECH_UNIQUE_PTR_HANDLE_H

#include <memory>
#include "handle.h"

namespace speech
{
    namespace handle
    {
        template<typename T>
        struct unique_ptr_handle : handle<T>
        {
            
            unique_ptr_handle(std::unique_ptr<T> sck) : handle<T>{ *sck.get() } , m_sck{ std::move(sck) }
            {   }

            private:

                std::unique_ptr<T> m_sck;
        };
    }
}

#endif
