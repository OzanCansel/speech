project(speech)
cmake_minimum_required(VERSION 3.12)

add_library(speech)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

target_sources(speech PUBLIC 
                        src/askfor.cpp
                        src/askfor.h
                        src/receiver.cpp
                        src/receiver.h
                        src/tcpreceiver.cpp
                        src/tcpreceiver.h
                        src/tcptransmitter.cpp
                        src/tcptransmitter.h
                        src/transmitter.cpp
                        src/transmitter.h
                        src/udpreceiver.cpp
                        src/udpreceiver.h
                        src/udptransmitter.cpp
                        src/udptransmitter.h
                        src/concepts_utility.h
                        src/typehash.h
                        src/namedtype.h
                        src/namedtype.cpp
)

target_link_libraries(speech Qt5::Core Qt5::Network)