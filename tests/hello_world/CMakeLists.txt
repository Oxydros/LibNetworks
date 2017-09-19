cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES src/*.cpp)
file(GLOB INCLUDES include/*.h)

add_executable(hello_world ${SOURCES} ${INCLUDES})

##target_include_directories(hello_world PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(hello_world cubznetworks)