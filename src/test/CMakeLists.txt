project( speech-test )

add_executable( test-serialize serialize.cpp )
add_executable( test-ask ask.cpp )

target_link_libraries( test-serialize speech::speech )
target_link_libraries( test-ask speech::speech )

add_test( NAME serialize
            COMMAND test-serialize -s )

add_test( NAME ask
                COMMAND test-ask -s )
