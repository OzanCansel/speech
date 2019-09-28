project( speech )
cmake_minimum_required(VERSION 3.1)
project( speech
        VERSION 0.2.0 )


include(CTest)
include(CPack)

add_subdirectory(src)

set(CPACK_PACKAGE_NAME "speech lib")
set(CPACK_PACKAGE_VENDOR "No Vendor")
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
set(CPACK_HOMEPAGE_URL "www.github.com/OzanCansel/speech")
set(CPACK_PACKAGE_CONTACT "ozancansel@gmail.com")
set(CPACK_RESOURCE_FILE_LICENCE ${CMAKE_CURRENT_SOURCE_DIR}/LICENCE.md)
set(CPACK_GENERATOR "ZIP;DEB")
