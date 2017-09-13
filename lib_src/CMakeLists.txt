cmake_minimum_required(VERSION 3.5)
project(cubznetworks)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_definitions( -DBOOST_ALL_NO_LIB )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.51.0 COMPONENTS filesystem system thread regex REQUIRED)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    file(GLOB SOURCES src/*.cpp)
    file(GLOB INCLUDES include/*.h)

    add_library(cubznetworks STATIC ${SOURCES} ${INCLUDES})

    target_include_directories(cubznetworks PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries(cubznetworks ${Boost_LIBRARIES})
endif()