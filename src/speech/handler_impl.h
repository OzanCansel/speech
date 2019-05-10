namespace speech
{

        template<typename Entity, template<typename...> class Receiver>
        template<typename... Params>
        handler<Entity , Receiver>::handler(callback_t f ,
                                    Params&... params)
                                    :
            Receiver<Entity>{ params... } ,
            m_callback{ f }
        {   }

        template<typename Entity, template<typename...> class Receiver>
        void handler<Entity , Receiver>::on_receive(const Entity& e)
        {
            m_callback(e);

            using base = Receiver<Entity>;
            base::messages(speech::impl::identifier<Entity >{});
        }
}