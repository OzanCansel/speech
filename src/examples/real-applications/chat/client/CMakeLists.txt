project( chat-client )
cmake_minimum_required(VERSION 3.1)

find_package( Qt5Widgets )

if ( NOT Qt5Widgets_FOUND )

    message( WARNING "QtWidgets module could not found, so chat-client example will not be compiled" )

else()

    add_executable( chat-client main.cpp send_event_text_edit.h send_event_text_edit.cpp server_comm.h server_comm.cpp )

    target_link_libraries( chat-client Qt5::Widgets speech::speech messages::messages )

    set_target_properties( chat-client PROPERTIES
        AUTOMOC ON
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )

    install(TARGETS chat-client
                    RUNTIME DESTINATION bin/examples/real-applications)

endif()
