cmake_minimum_required (VERSION 2.8.8)

project (secure_hello)

include_directories( ${CMAKE_SOURCE_DIR}/secure_hello )

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/*.proto")
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtoFiles})
PROTOBUF_GENERATE_GRPC_CPP(GRPC_SRCS GRPC_HDRS ${ProtoFiles})

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

add_executable(secure_hello_server ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS} server.cpp server1_cert.c server1_key.c test_root_cert.c)
add_executable(secure_hello_client ${PROTO_SRCS} ${PROTO_HDRS} ${GRPC_SRCS} ${GRPC_HDRS} client.cpp server1_cert.c server1_key.c test_root_cert.c)

target_link_libraries(secure_hello_server ${CMAKE_THREAD_LIBS_LINIT} ${PROTOBUF_LIBRARIES} ${GRPC_LIBRARIES})
target_link_libraries(secure_hello_client ${CMAKE_THREAD_LIBS_LINIT} ${PROTOBUF_LIBRARIES} ${GRPC_LIBRARIES})


