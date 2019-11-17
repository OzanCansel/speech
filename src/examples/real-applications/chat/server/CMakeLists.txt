project(chat-server)
cmake_minimum_required(VERSION 3.1)

add_executable( chat-server main.cpp )

target_link_libraries( chat-server speech::speech )
target_link_libraries( chat-server messages::messages )

set_target_properties( chat-server PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
