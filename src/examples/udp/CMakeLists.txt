project(udp)
cmake_minimum_required(VERSION 3.1)

add_executable(transmit transmit.cpp)
add_executable(receive-with-inheritance receive_with_inheritance.cpp)
add_executable(receive-from-queue receive_from_queue.cpp)
add_executable(transmit-function transmit_function.cpp)

target_link_libraries(transmit speech model::model)
target_link_libraries(receive-with-inheritance speech model::model)
target_link_libraries(receive-from-queue speech model::model)
target_link_libraries(transmit-function speech model::model)

set_target_properties(transmit receive-with-inheritance receive-from-queue transmit-function PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS transmit receive-with-inheritance receive-from-queue
		RUNTIME DESTINATION bin/examples/udp)
