#ifndef IS_CONTAINER_H
#define IS_CONTAINER_H

#include <vector>
#include <list>

namespace speech
{

    template <typename Container>
    struct is_container : std::false_type { };

    template <typename... Ts> struct is_container<std::list<Ts...> > : std::true_type { };
    template <typename... Ts> struct is_container<std::vector<Ts...> > : std::true_type { };

}

#endif // IS_CONTAINER_H
