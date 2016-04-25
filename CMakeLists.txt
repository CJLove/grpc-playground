cmake_minimum_required (VERSION 2.8.8)

project (grpc)

set (CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH}
                        ${CMAKE_CURRENT_SOURCE_DIR}/CMake)

find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)

add_definitions(-std=c++11)

# Simple hello-world
add_subdirectory(hello)



