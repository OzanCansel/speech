#ifndef SPEECH_HANDLER_H
#define SPEECH_HANDLER_H

#include <functional>
#include "shared_socket.h"
#include "speech/tcp/tcp_receiver.h"

namespace speech
{

        template<typename Entity, template<typename...> class Receiver>
        class handler_f : public Receiver<Entity>
        {
            public:

                using callback_t = std::function<void(const Entity& , typename Receiver<Entity>::socket_type&)>;

                template<typename... Params>
                handler_f(callback_t , Params&...);

            protected:

                void on_receive(const Entity&) override;

            private:

                callback_t m_callback;

        };
};

#include "handler_f_impl.h"

#endif