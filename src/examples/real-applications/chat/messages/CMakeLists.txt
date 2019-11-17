project(chat-messages)
cmake_minimum_required(VERSION 3.1)

add_library(messages INTERFACE)
add_library(messages::messages ALIAS messages)

target_include_directories(messages INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
