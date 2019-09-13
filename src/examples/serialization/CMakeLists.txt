project(serialization)
cmake_minimum_required(VERSION 3.1)

add_executable(serialize-transmit serialize-transmit.cpp
                                    car_info.h car_info.cpp
                                    rolls.h rolls.cpp)
add_executable(serialize-receive serialize-receive.cpp
                                    car_info.h car_info.cpp
                                    rolls.h rolls.cpp)

set_target_properties(serialize-transmit serialize-receive PROPERTIES AUTOMOC TRUE)

target_link_libraries(serialize-transmit speech::speech model::model)
target_link_libraries(serialize-receive speech::speech model::model)

set_target_properties(serialize-transmit serialize-receive PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS serialize-transmit serialize-receive
		RUNTIME DESTINATION bin/examples/qobject)
