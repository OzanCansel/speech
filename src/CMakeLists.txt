cmake_minimum_required(VERSION 3.1)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

add_library(speech INTERFACE)

add_library(speech::speech ALIAS speech)

target_link_libraries(speech INTERFACE Qt5::Core Qt5::Network)

target_include_directories(speech INTERFACE  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>  
    $<INSTALL_INTERFACE:include>  # <prefix>/include/mylib
)

include(GNUInstallDirs)

install(TARGETS speech
    EXPORT speech-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/speech
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)

install(EXPORT speech-targets
        FILE speech-targets.cmake
        NAMESPACE speech::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speech
)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/speech-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/speech-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speech
)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/speech-config-version.cmake
    VERSION 3.1
    COMPATIBILITY AnyNewerVersion
)

install(FILES   
        ${CMAKE_BINARY_DIR}/cmake/speech-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/speech-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/speech
)

export(EXPORT speech-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/speech-targets.cmake
    NAMESPACE speech::
)

add_subdirectory(examples)