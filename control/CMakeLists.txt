cmake_minimum_required(VERSION 3.30)
project(zinpassctl VERSION 0.0.2 LANGUAGES CXX)

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "A:/build/grpc-1.71.0-amd64-win-mingw64"    # gRPC-1.71.0
)

# 查找依赖包
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置变量，包含项目文件
set(PROJECT_SOURCES
        # 0. 主函数
        src/main.cpp

        # 1. 命令行交互-cli
        src/cli/prompt.cpp
        src/cli/read_eval_print_loop.cpp
        src/cli/command_handler.cpp
        src/cli/cli_command.cpp

        # 2. 通信-grpc
        src/rpc/status_rpc.cpp

        #generated
        generate/common/v1/common.pb.cc
        generate/common/v1/common.grpc.pb.cc
        generate/control/status.pb.cc
        generate/control/status.grpc.pb.cc
        generate/control/status.grpc.pb.h
)

# 添加可执行文件
add_executable(zinpassctl
        ${PROJECT_SOURCES}
)

# 包含头文件目录（在add_executable、add_library之后）
target_include_directories(zinpassctl
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include/cli
        ${CMAKE_SOURCE_DIR}/include/rpc

        ${CMAKE_SOURCE_DIR}/generate
        ${CMAKE_SOURCE_DIR}/generate/common
        ${CMAKE_SOURCE_DIR}/generate/common/v1
        ${CMAKE_SOURCE_DIR}/generate/control
        ${CMAKE_SOURCE_DIR}/generate/hello
)

# 链接库
target_link_libraries(zinpassctl PRIVATE
        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf
)