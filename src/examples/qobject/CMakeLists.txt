project(qobject)
cmake_minimum_required(VERSION 3.1)

add_executable(auto_serialize auto_serialize.cpp entity.h entity.cpp)
add_executable(qobject_receive qobject_receive.cpp entity.h entity.cpp)

set_target_properties(auto_serialize PROPERTIES AUTOMOC TRUE)
set_target_properties(qobject_receive PROPERTIES AUTOMOC TRUE)

target_link_libraries(auto_serialize speech)
target_link_libraries(qobject_receive speech)

install(TARGETS auto_serialize
		RUNTIME DESTINATION bin/examples/qobject)
install(TARGETS qobject_receive
		RUNTIME DESTINATION bin/examples/qobject)