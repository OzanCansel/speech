#ifndef SPEECH_HANDLE_H
#define SPEECH_HANDLE_H

namespace speech
{
    namespace handle
    {
        template<typename T>
        struct handle
        {
                handle(T& sck)  :   m_sck{ sck }
                {   }

                inline T& ref()
                {
                    return m_sck;
                }
                
                inline operator T&()
                {
                    return m_sck;
                }

            private:
                T& m_sck;
        };
    }
}

#endif