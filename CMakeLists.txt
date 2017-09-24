cmake_minimum_required (VERSION 3.5)
project (LibNetworks)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
    add_definitions(-D_DEBUG)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ENDIF()

add_subdirectory(lib_src)
add_subdirectory(tests/server)
add_subdirectory(tests/client)