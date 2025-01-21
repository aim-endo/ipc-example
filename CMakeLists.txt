cmake_minimum_required(VERSION 3.20)

project(ipc-example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# gRPC and protobuf
FetchContent_Declare(
  gRPC
  GIT_REPOSITORY https://github.com/grpc/grpc.git
  GIT_TAG v1.58.0
)
FetchContent_MakeAvailable(gRPC)

FetchContent_Declare(
  protobuf
  GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
  GIT_TAG v3.19.0
)
FetchContent_MakeAvailable(protobuf)

# libsystemd
FetchContent_Declare(
  libsystemd
  GIT_REPOSITORY https://github.com/systemd/systemd.git
  GIT_TAG v253
)
FetchContent_MakeAvailable(libsystemd)

# UCX
FetchContent_Declare(
  ucx
  GIT_REPOSITORY https://github.com/openucx/ucx.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(ucx)

#[[
set(UCX_INSTALL_DIR "${CMAKE_BINARY_DIR}/ucx-install")
if (NOT EXISTS "${UCX_INSTALL_DIR}")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E make_directory ${UCX_INSTALL_DIR}
    )
    execute_process(
      COMMAND ./autogen.sh
      WORKING_DIRECTORY ${ucx_SOURCE_DIR}
    )
    execute_process(
      COMMAND ./configure --prefix=${UCX_INSTALL_DIR} --without-systemd
      WORKING_DIRECTORY ${ucx_SOURCE_DIR}
    )
    execute_process(
      COMMAND make -j
      WORKING_DIRECTORY ${ucx_SOURCE_DIR}
    )
    execute_process(
      COMMAND make install
      WORKING_DIRECTORY ${ucx_SOURCE_DIR}
    )
endif()
set(Ucx_DIR "${UCX_INSTALL_DIR}/lib/cmake/ucx")
set(Ucx_DIR "${ucx_SOURCE_DIR}/cmake")
]]

# Add library target
add_library(ipc_mechanism STATIC
  src/ipc_base.cpp
  src/shared_memory.cpp
  src/grpc.cpp
  src/ucx.cpp
)

target_include_directories(ipc_mechanism PUBLIC include)
#target_include_directories(ipc_mechanism PUBLIC ${libsystemd_SOURCE_DIR}/src)

find_package(Threads REQUIRED)

# link gRPC
set(_GRPC_GRPCPP_UNSECURE gRPC::grpc++_unsecure)
target_link_libraries(ipc_mechanism PUBLIC ${_GRPC_GRPCPP_UNSECURE_LIB} Threads::Threads protobuf::libprotobuf)

# link UCX
#find_package(Ucx REQUIRED PATHS "${Ucx_DIR}" NO_DEFAULT_PATH)
#target_link_libraries(ipc_mechanism PUBLIC Ucx::ucx)
#target_link_libraries(ipc_mechanism PUBLIC ${libsystemd_SOURCE_DIR}/src/libsystemd.a)

# Add daemon executable
add_executable(ipcd src/daemon.cpp)
target_link_libraries(ipcd PRIVATE ipc_mechanism)

# Add client executable
add_executable(ipccli src/client.cpp)
target_link_libraries(ipccli PRIVATE ipc_mechanism)
