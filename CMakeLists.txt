cmake_minimum_required (VERSION 3.5)
project (LibNetworks)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    add_definitions(-D_DEBUG)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ENDIF()

add_subdirectory(lib_src)
add_subdirectory(proto)
add_subdirectory(tests/server)
add_subdirectory(tests/client)