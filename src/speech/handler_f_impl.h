namespace speech
{

        template<typename Entity, template<typename...> class Receiver>
        template<typename... Params>
        handler_f<Entity , Receiver>::handler_f(callback_t f ,
                                    Params&... params)
                                    :
            Receiver<Entity>{ params... } ,
            m_callback{ f }
        {   }

        template<typename Entity, template<typename...> class Receiver>
        void handler_f<Entity , Receiver>::on_receive(const Entity& e)
        {
            m_callback(e , Receiver<Entity>::device());
        }
}