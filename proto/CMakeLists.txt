cmake_minimum_required (VERSION 3.5)

set(CMAKE_CXX_STANDARD 14)

INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER TCPMessage.proto)
ADD_LIBRARY(proto STATIC ${PROTO_HEADER} ${PROTO_SRC})

message(STATUS "ProtoLib ${PROTOBUF_LIBRARY}")

target_include_directories(proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(proto ${PROTOBUF_LIBRARY})