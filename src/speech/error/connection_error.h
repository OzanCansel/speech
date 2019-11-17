#ifndef SPEECH_CONNECTION_ERROR
#define SPEECH_CONNECTION_ERROR

#include <exception>
#include <string>

namespace speech
{

namespace error
{

struct connection_error : std::runtime_error {
     explicit connection_error ( std::string&& msg ) : std::runtime_error { std::forward<std::string>( msg ) }
     {   }
};

}

}

#endif
