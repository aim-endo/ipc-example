
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(PROTO_FILES
    ${PROTO_SRC_DIR}/hello.proto
)

add_library(protolib ${PROTO_FILES})
target_link_libraries(protolib PUBLIC
    protobuf::libprotobuf
    gRPC::grpc
    gRPC::grpc++
)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")
