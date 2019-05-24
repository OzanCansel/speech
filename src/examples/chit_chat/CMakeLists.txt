project(chit_chat)
cmake_minimum_required(VERSION 3.1)

add_executable(john john.cpp questions.h)
add_executable(bjarne bjarne.cpp questions.h)

target_link_libraries(john speech::speech)
target_link_libraries(bjarne speech::speech)

set_target_properties(john bjarne PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS john bjarne
        RUNTIME DESTINATION bin/examples/tcp)
