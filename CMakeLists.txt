cmake_minimum_required(VERSION 3.20)

project(ipc-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Ucx REQUIRED)
find_package(Threads REQUIRED)

# Add library target
add_library(ipc_mechanism STATIC
  src/ipc_base.cpp
  src/shared_memory.cpp
  src/grpc.cpp
  src/ucx.cpp
)

target_include_directories(ipc_mechanism PUBLIC include)
target_link_libraries(ipc_mechanism PUBLIC 
    Threads::Threads 
    gRPC::grpc++ 
    protobuf::libprotobuf
    ucm ucp ucs uct
)

# Add protobuf target
add_subdirectory(proto)

# Add daemon executable
add_executable(ipcd src/daemon.cpp)
target_include_directories(ipcd PUBLIC include)
target_link_libraries(ipcd PRIVATE ipc_mechanism protolib)

# Add client executable
add_executable(ipc-cli src/client.cpp)
target_include_directories(ipc-cli PUBLIC include)
target_link_libraries(ipc-cli PRIVATE ipc_mechanism protolib)
