cmake_minimum_required(VERSION 3.20)

project(client-desktop VERSION 0.0.1 LANGUAGES CXX)

# 设置 Qt 库路径（mingw64静态库）
set(CMAKE_PREFIX_PATH
        "A:/build/qt-6.9.0-win64-mingw64-static"
        "A:/build/grpc-1.71.0-amd64-win-mingw64"
)

# 设置 gRPC 和 Protobuf 的根目录
set(GRPC_INSTALL_DIR "A:/build/grpc-1.71.0-amd64-win-mingw64")
set(Protobuf_ROOT ${GRPC_INSTALL_DIR})
set(gRPC_ROOT ${GRPC_INSTALL_DIR})

# 设置 Protobuf 的 CMake 配置路径
set(Protobuf_DIR "${GRPC_INSTALL_DIR}/lib/cmake/protobuf")

# 添加自定义 CMake 模块路径（包含 grpc 的 Config.cmake）
list(APPEND CMAKE_MODULE_PATH "${GRPC_INSTALL_DIR}/cmake")

# 启用自动处理moc/uic/rcc
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/generate/qt/ui) # 创建 ui_*.h 生成目录
#set(CMAKE_AUTOUIC_OPTIONS "-o ${CMAKE_SOURCE_DIR}/generate/qt/ui") # 全局设置uic输出目录

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
# find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 查找 Protobuf 和 gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 包含头文件目录
include_directories(
        ${OPENSSL_INCLUDE_DIR} # 添加 OpenSSL 头文件路径
        ${SQLite3_INCLUDE_DIR} # 添加 SQLite3 头文件路径
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/state
        ${CMAKE_SOURCE_DIR}/include/storage
        ${CMAKE_SOURCE_DIR}/include/rpc
        ${CMAKE_SOURCE_DIR}/include/business
        ${CMAKE_SOURCE_DIR}/include/models # 用户侧模型

        ${CMAKE_SOURCE_DIR}/src/ui
        ${CMAKE_SOURCE_DIR}/src/ui/account
        ${CMAKE_SOURCE_DIR}/src/ui/phone
        ${CMAKE_SOURCE_DIR}/src/ui/user
        ${CMAKE_SOURCE_DIR}/src/ui/pwd

        ${CMAKE_SOURCE_DIR}/generate
        ${CMAKE_SOURCE_DIR}/generate/grpc
        ${CMAKE_SOURCE_DIR}/generate/grpc/user/v1
        ${CMAKE_SOURCE_DIR}/generate/grpc/account/v1
        ${CMAKE_SOURCE_DIR}/generate/grpc/common/v1
        # ${CMAKE_SOURCE_DIR}/generate/qt/ui        # 添加头文件搜索路径
)

set(PROJECT_SOURCES
        # 1. 主函数
        src/main.cpp

        # 2. 应用运行时状态管理层（全局变量、跨组件的共享状态、网络请求的临时上下文、当前用户对象、UI主题状态、临时缓存） state
        src/state/state_manager.cpp

        # 3. 本地持久化存储抽象层（用户偏好设置、本地缓存、加密的会话令牌、设备标识等）storage
        # 暂无

        # 4. 用户侧模型 Models
        src/models/user.cpp
        src/models/session.cpp
        src/models/mobile_phone.cpp
        src/models/account.cpp
        src/models/view_account.cpp

        # 5. utils 层
        src/business/date_time.cpp
        # src/business/pwd_utils.cpp
        src/business/pwd_generator.cpp

        # 6. UI 层
        # (6.0) 资源文件
        res/resource.qrc
        res/icon_zinpass.rc # windows平台pe文件图标
        # (6.1) 常用模块与公共组件
        src/ui/search_box.cpp
        src/ui/is_input_valid.cpp # 判断输入的数据是否合法-前端部分辅助验证
        src/ui/dialog_auth.cpp # 验证当前用户身份
        src/ui/widget_public_method.cpp # 部件公用方法
        # (6.2) 系统登录界面、系统用户注册界面
        src/ui/user/login_dlg.cpp
        src/ui/user/login_dlg.ui
        src/ui/user/signup_dlg.cpp
        src/ui/user/signup_dlg.ui
        src/ui/user/personal_mgr_form.cpp
        src/ui/user/personal_mgr_form.ui
        src/ui/user/cancel_personal_user_form.cpp
        # (6.3) 主工作台
        src/ui/main_workbench.cpp
        src/ui/main_workbench.ui
        # (6.4) 主功能菜单
        src/ui/function_menu_form.cpp
        src/ui/function_menu_form.ui
        # (6.5) 管理账号：页面框架、添加和修改账号记录、修改账号所属人
        src/ui/account/account_filter_form.cpp
        src/ui/account/account_mgr_form.cpp
        src/ui/account/popup_filter_window.cpp
        src/ui/account/dialog_exposed_pwd.cpp # 查看明文密码的弹窗
        src/ui/account/dialog_add_account.cpp
        src/ui/account/dialog_edit_account.cpp
        src/ui/account/dialog_delete_account.cpp
        src/ui/account/data_owner_form.cpp
        # (6.6) 管理手机号：页面框架、增加手机号
        src/ui/phone/phone_filter_form.cpp
        src/ui/phone/phone_detail_frame.cpp
        src/ui/phone/dialog_add_phone.cpp
        src/ui/phone/phone_mgr_form.cpp
        # (6.7) 密码相关页面：随机密码生成器、密码安全检查器
        src/ui/pwd/pwd_gen_dlg.cpp
        src/ui/pwd/pwd_generator_dlg.cpp
        src/ui/pwd/check_pwd_security_form.cpp
        src/ui/pwd/check_pwd_security_form.ui
        # (6.8) 迁移备份数据
        src/ui/output_form.cpp
        src/ui/input_form.cpp

        # 7、RPC 层
        # (7.1) 接口
        src/rpc/user_rpc.cpp
        src/rpc/auth_rpc.cpp
        src/rpc/telecom_rpc.cpp
        src/rpc/phone_rpc.cpp
        src/rpc/account_rpc.cpp
        src/rpc/category_rpc.cpp
        # (7.2) generated
        generate/grpc/common/v1/common.pb.cc
        generate/grpc/common/v1/common.grpc.pb.cc
        generate/grpc/hello/hello.pb.cc
        generate/grpc/hello/hello.grpc.pb.cc
        generate/grpc/user/v1/user.pb.cc
        generate/grpc/user/v1/user.grpc.pb.cc
        generate/grpc/user/v1/auth.pb.cc
        generate/grpc/user/v1/auth.grpc.pb.cc
        generate/grpc/account/v1/telecom.pb.cc
        generate/grpc/account/v1/telecom.grpc.pb.cc
        generate/grpc/account/v1/phone.pb.cc
        generate/grpc/account/v1/phone.grpc.pb.cc
        generate/grpc/account/v1/account.pb.cc
        generate/grpc/account/v1/account.grpc.pb.cc
        generate/grpc/account/v1/category.pb.cc
        generate/grpc/account/v1/category.grpc.pb.cc
)

add_executable(client-desktop
        ${PROJECT_SOURCES}
)

# 链接 Qt6 和 OpenSSL、Windows 系统库
target_link_libraries(client-desktop PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets

        # gRPC相关库
        gRPC::grpc++
        gRPC::grpc
        #gRPC::grpc_cronet
        gRPC::gpr
        protobuf::libprotobuf

        ws2_32
        user32
        advapi32
        crypt32
)

# 设置Win32应用、优化二进制体积、静态编译c++运行时库、线程库、设置可执行文件输出路径
set_target_properties(client-desktop PROPERTIES
        WIN32_EXECUTABLE TRUE  # Win32 应用，而非 console 应用
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin # 设置可执行文件的输出路径
        LINK_FLAGS "-static -Wl,-s"
)

# 静态编译专用配置
target_compile_definitions(client-desktop PRIVATE
        QT_STATICPLUGIN  # 启用静态插件支持
)

# 安装配置
include(GNUInstallDirs)
install(TARGETS client-desktop
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 构建命令示例
# mkdir build && cd build
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ -DCMAKE_MAKE_PROGRAM=mingw32-make ..
# mingw32-make -j16
