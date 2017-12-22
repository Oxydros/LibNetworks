#!/bin/bash

PROTO_CMAKE="cmake_minimum_required (VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
message(STATUS \"ProtoLib \${PROTOBUF_LIBRARIES}\")
message(STATUS \"ProtoInclude \${PROTOBUF_INCLUDE_DIRS}\")
file(GLOB PROTO_FILES_SOURCES *.proto)
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS \${PROTO_FILES_SOURCES})
ADD_LIBRARY(proto STATIC \${PROTO_HDRS} \${PROTO_SRCS})
target_include_directories(proto PUBLIC \${PROTOBUF_INCLUDE_DIR} \${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto \${PROTOBUF_LIBRARIES})
set(EXTERNAL_LIBS \${PROTOBUF_LIBRARIES} PARENT_SCOPE)
set(EXTERNAL_DEPS_INCLUDES \${PROTOBUF_INCLUDE_DIR} PARENT_SCOPE)
set(EXTERNAL_INCLUDE_DIR \${CMAKE_CURRENT_BINARY_DIR} PARENT_SCOPE)
set(PROTO_SOURCES \${PROTO_SRCS} PARENT_SCOPE)
set(PROTO_HEADERS \${PROTO_HDRS} PARENT_SCOPE)
"
PROTO_CMAKELIST_FILE="$PWD/lib_src/proto/CMakeLists.txt"

echo "Initializing submodules"
git submodule sync --recursive
git submodule update --init --remote --merge

echo "Creating proto CMakeLists.txt"
echo "$PROTO_CMAKE" > $PROTO_CMAKELIST_FILE
