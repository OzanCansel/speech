project(real-applications)
cmake_minimum_required(VERSION 3.1)

add_executable(discovery discovery.cpp)

target_link_libraries(discovery speech model::model)

set_target_properties(discovery PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

install(TARGETS discovery
		RUNTIME DESTINATION bin/examples/real-applications)
