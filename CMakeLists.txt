cmake_minimum_required (VERSION 3.5)

if(MSCV)
	set(CMAKE_USER_MAKE_RULES_OVERRIDE
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/c_flag_overrides.cmake)
	set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX
		${CMAKE_CURRENT_SOURCE_DIR}/cmake/cxx_flag_overrides.cmake)
endif(MSCV)
project (LibNetworks)

if(MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
endif()

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
add_subdirectory(tests/tcpServer)
add_subdirectory(tests/tcpClient)
add_subdirectory(tests/tcpClientMultiThread)

add_subdirectory(tests/udpClient)
add_subdirectory(tests/udpServer)
