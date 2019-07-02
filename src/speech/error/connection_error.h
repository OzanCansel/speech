#ifndef SPEECH_CONNECTION_ERROR
#define SPEECH_CONNECTION_ERROR

#include <exception>

namespace speech
{

namespace error
{

struct connection_error : std::runtime_error {
     connection_error ( std::string msg ) : std::runtime_error{msg}
     {   }
};

}

}

#endif
