project( speech-test )

add_executable( test-serialize serialize.cpp )

target_link_libraries( test-serialize speech::speech )

add_test( NAME serialize
            COMMAND test-serialize -s )
