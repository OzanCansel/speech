project(tcp)
cmake_minimum_required(VERSION 3.1)

add_executable(feed feed.cpp)
add_executable(server server.cpp)
add_executable(server-with-functors server-with-functors.cpp)
add_executable(feed-function feed-function.cpp)

target_link_libraries(feed speech::speech model::model)
target_link_libraries(server speech::speech model::model)
target_link_libraries(server-with-functors speech::speech model::model)
target_link_libraries(feed-function speech::speech model::model)

set_target_properties(server server-with-functors feed feed-function PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS server server-with-functors feed
        RUNTIME DESTINATION bin/examples/tcp)
