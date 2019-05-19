#ifndef SPEECH_SHARED_PTR_SCK_H
#define SPEECH_SHARED_PTR_SCK_H

#include <memory>
#include "handle.h"

namespace speech
{
    namespace handle
    {
        template<typename  T>
        struct shared_ptr_handle : handle<T>
        {
            shared_ptr_handle(std::shared_ptr<T> sck) : handle<T>(*sck.get()) , m_sck{  sck }
            {   }
            private:
                std::shared_ptr<T> m_sck;
        };
    }
}

#endif