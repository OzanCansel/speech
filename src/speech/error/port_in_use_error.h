#ifndef SPEECH_PORT_IN_USE_ERROR_H
#define SPEECH_PORT_IN_USE_ERROR_H

#include <exception>

namespace speech
{

namespace error
{

struct port_in_use_error : std::runtime_error {
     port_in_use_error ( std::string msg ) : std::runtime_error{msg}
     {   }
};

}

}

#endif
