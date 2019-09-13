#ifndef IS_CONTAINER_H
#define IS_CONTAINER_H

#include <vector>
#include <list>

namespace speech
{

    template <typename S>
    class has_begin_and_end
    {

        template <typename SS>
        static auto test_begin(int)
            -> decltype(std::declval<SS>().begin() , std::true_type());

        template <typename>
        static auto test_begin(...) -> std::false_type;

        template <typename SS>
        static auto test_end(int)
            -> decltype ( std::declval<SS>().end() , std::true_type() );

        template <typename>
        static auto test_end(...) -> std::false_type;

      public:

        static const bool value = decltype(test_begin<S>(0))::value && decltype (test_end<S>(0))::value;

    };

    template <typename S>
    class has_size
    {
        template<typename SS>
        static auto test(int)
            -> decltype ( std::declval<SS>().size() , std::true_type() );

        template<typename>
        static auto test(...) -> std::false_type;

    public:

        static const bool value = decltype ( test<S>(0) )::value;

    };

    template <typename S>
    class has_push_back
    {
        template<typename SS>
        static auto test(int)
            -> decltype( std::declval<SS>().push_back( std::declval<typename SS::value_type>() ) , std::true_type{} );

        template<typename>
        static auto test(...) -> std::false_type;


    public:

        static const bool value = decltype( test<S>(0) )::value;

    };


    template <typename S>
    class has_at
    {
        template<typename SS>
        static auto test(int)
            -> decltype( std::declval<SS>().at( 0 ) , std::true_type{} );

        template<typename>
        static auto test(...) -> std::false_type;

    public:

        static const bool value = decltype( test<S>(0) )::value;

    };

    template<typename Cont>
    struct is_container
    {
        static const bool value = has_begin_and_end<Cont>::value && has_size<Cont>::value && has_push_back<Cont>::value && has_at<Cont>::value;
    };

}

#endif // IS_CONTAINER_H
