cmake_minimum_required (VERSION 3.6)
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER test.proto)
ADD_LIBRARY(proto ${PROTO_HEADER} ${PROTO_SRC})

target_include_directories(proto PUBLIC .)
target_link_libraries(proto ${PROTOBUF_LIBRARY})