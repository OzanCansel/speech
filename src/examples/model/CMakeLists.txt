project(model)
cmake_minimum_required(VERSION 3.1)

add_library(model INTERFACE)
add_library(model::model ALIAS model)

target_include_directories(model INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)