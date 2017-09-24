cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif(UNIX)

file(GLOB SOURCES src/*.cpp)
file(GLOB INCLUDES include/*.h)

add_executable(server ${SOURCES} ${INCLUDES})

##target_include_directories(hello_world PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(server cubznetworks)