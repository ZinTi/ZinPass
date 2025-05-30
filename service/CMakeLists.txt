cmake_minimum_required(VERSION 3.30)
project(zinpass_service VERSION 0.0.2 LANGUAGES CXX)

# 设置平台相关变量
if(WIN32)
    set(EXECUTABLE_NAME "ZinPassSvc")    # Windows下生成 ZinPassSvc.exe
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(EXECUTABLE_NAME "zinpassd")      # Linux下生成 zinpassd
else()
    set(EXECUTABLE_NAME "zinpass_service") # 其他平台保留默认名称
endif()

# 设置依赖库搜索路径（完整路径、环境变量或缓存变量）
list(APPEND CMAKE_PREFIX_PATH
        "A:/build/grpc-1.71.0-amd64-win-mingw64"    # gRPC-1.71.0
        # "A:/build/openssl-win64-mingw"              # OpenSSL-3.5.0 (gRPC 库中内置了 OpenSSL，所以使用 gRPC 内置的 OpenSSL 库)
        "A:/build/boost-msys2-mingw64"              # Boost-1.88
        "A:/build/sqlite-3.49.1-mingw64-static"     # SQLite-3.49.1 (libsqlite3.a)
)

# 查找依赖包
find_package(OpenSSL 3.5.0 REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(Boost 1.88 REQUIRED)
find_package(SQLite3 REQUIRED)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 指定Boost头文件库路径
set(BOOST_HEADER_ONLY_DIR "A:/build/boost-msys2-mingw64/include/boost-1_88")

# 设置变量，包含项目文件
set(PROJECT_SOURCES
        # 0. 后台服务/守护进程程序主函数
        src/main.cpp

        # 1. 通用工具-utils
        src/utils/date_time.cpp
        src/utils/pwd_utils.cpp
        src/utils/pwd_generator.cpp
        src/utils/uuid_generator.cpp
        src/utils/log_manager.cpp           # 日志

        # 2. 服务配置-config
        src/config/about.cpp
        src/config/read_configuration_file.cpp
        src/config/state_manager.cpp

        # 3. 业务逻辑层-business
        src/business/user_manager.cpp
        src/business/session_manager.cpp
        src/business/telecom_operator_manager.cpp
        src/business/mobile_phone_manager.cpp
        src/business/account_manager.cpp
        src/business/authentication.cpp

        # 4. 数据访问层-repository
        src/repository/connection_pool.cpp
        src/repository/common/sql_debug.cpp
        src/repository/init_database.cpp # 初始化 Database
        src/repository/system_user_dao.cpp
        src/repository/session_dao.cpp
        src/repository/telecom_operator_dao.cpp
        src/repository/mobile_phone_dao.cpp
        src/repository/category_dao.cpp
        src/repository/account_dao.cpp

        # 5. 领域模型（与proto解耦）-models
        src/models/system_user.cpp
        src/models/session.cpp
        src/models/mobile_phone.cpp
        src/models/account.cpp
        src/models/view_account.cpp

        # 6. gRPC 服务实现层-service
        src/service/grpc_client_service.cpp
        src/service/grpc_control_service.cpp
        src/service/hello_service.cpp       # 测试 RPC 是否连通
        src/service/user/user_service.cpp
        src/service/user/auth_service.cpp
        src/service/phone/telecom_service.cpp
        src/service/phone/phone_service.cpp
        src/service/account/account_service.cpp
        src/service/account/category_service.cpp
        src/service/status/status_service.cpp

        # generated
        generate/common/v1/common.pb.cc
        generate/common/v1/common.grpc.pb.cc
        generate/hello/hello.pb.cc
        generate/hello/hello.grpc.pb.cc
        generate/control/status.pb.cc
        generate/control/status.grpc.pb.cc

        generate/user/v1/user.pb.cc
        generate/user/v1/user.grpc.pb.cc
        generate/user/v1/auth.pb.cc
        generate/user/v1/auth.grpc.pb.cc
        generate/account/v1/telecom.pb.cc
        generate/account/v1/telecom.grpc.pb.cc
        generate/account/v1/phone.pb.cc
        generate/account/v1/phone.grpc.pb.cc
        generate/account/v1/account.pb.cc
        generate/account/v1/account.grpc.pb.cc
        generate/account/v1/category.pb.cc
        generate/account/v1/category.grpc.pb.cc
)

# 添加可执行文件
add_executable(zinpass_service
        ${PROJECT_SOURCES}
)

# 设置输出文件名
set_target_properties(zinpass_service PROPERTIES
        OUTPUT_NAME "${EXECUTABLE_NAME}"
)

# 包含头文件目录（在add_executable、add_library之后）
target_include_directories(zinpass_service
    PRIVATE
        ${CMAKE_SOURCE_DIR}/internal
        ${CMAKE_SOURCE_DIR}/internal/utils
        ${CMAKE_SOURCE_DIR}/internal/config
        ${CMAKE_SOURCE_DIR}/internal/models
        ${CMAKE_SOURCE_DIR}/internal/repository
        ${CMAKE_SOURCE_DIR}/internal/business

        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/service

        ${CMAKE_SOURCE_DIR}/generate/

        ${OPENSSL_INCLUDE_DIR} # OpenSSL 头文件
        ${BOOST_HEADER_ONLY_DIR}  # Boost 头文件
)

# 仅Windows平台设置无控制台
if(WIN32)
    # 判断编译器类型
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-subsystem,windows") # MinGW-w64/GCC/Clang使用 -Wl,-subsystem,windows
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS") # MSVC使用 /SUBSYSTEM:WINDOWS
    endif()
endif()

# 链接库
target_link_libraries(zinpass_service
    PRIVATE
        SQLite::SQLite3

        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf
)