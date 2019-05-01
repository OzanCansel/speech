project(udp)
cmake_minimum_required(VERSION 3.1)

add_executable(transmit transmit.cpp models.h)
add_executable(receive-with-inheritance receive_with_inheritance.cpp models.h)
add_executable(receive-from-queue receive_from_queue.cpp models.h)

target_link_libraries(transmit speech)
target_link_libraries(receive-with-inheritance speech)
target_link_libraries(receive-from-queue speech)

install(TARGETS transmit
		RUNTIME DESTINATION bin/examples/udp)

install(TARGETS receive-with-inheritance
		RUNTIME DESTINATION bin/examples/udp)

install(TARGETS receive-from-queue
		RUNTIME DESTINATION bin/examples/udp)