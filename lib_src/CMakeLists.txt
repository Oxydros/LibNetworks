cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(proto)

SET_SOURCE_FILES_PROPERTIES(${PROTO_SOURCES} ${PROTO_HEADERS} PROPERTIES GENERATED TRUE)

add_definitions( -DBOOST_ALL_NO_LIB )
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.51.0 COMPONENTS "filesystem" "system" "thread" "regex" REQUIRED)

if(Boost_FOUND)
    if (MSCV)
      add_definitions(-D_WIN32_WINNT=0x0501)
    endif(MSCV)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
	message(STATUS "External include dirs: ${EXTERNAL_DEPS_INCLUDES} ${EXTERNAL_INCLUDE_DIR}")
	message(STATUS "Proto SRCS: ${PROTO_SOURCES}")
	message(STATUS "Proto HDRS: ${PROTO_HEADERS}")

    file(GLOB SOURCES src/*.cpp)
    file(GLOB INCLUDES include/*.h)

    add_library(cubznetworks STATIC ${SOURCES} ${INCLUDES})

    target_include_directories(cubznetworks PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${Boost_INCLUDE_DIRS})

    target_link_libraries(cubznetworks proto ${Boost_LIBRARIES} ${EXTERNAL_LIBS})
endif()