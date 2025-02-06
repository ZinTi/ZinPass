cmake_minimum_required(VERSION 3.16)

project(ZinPass VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 只查找 Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 设置 OpenSSL 路径
set(OPENSSL_ROOT_DIR "A:/app/OpenSSL-Win64-MinGW")

# 查找 OpenSSL
find_package(OpenSSL REQUIRED)

# 如果找不到 OpenSSL，手动指定
if (NOT OpenSSL_FOUND)
    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
    set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libcrypto.a")
    set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libssl.a")
    set(OPENSSL_LIBRARIES ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
endif()

# 包含头文件目录
include_directories(
        ${OPENSSL_INCLUDE_DIR} # 添加 OpenSSL 头文件
        ${CMAKE_SOURCE_DIR}/lib
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/model
        ${CMAKE_SOURCE_DIR}/src/dao
        ${CMAKE_SOURCE_DIR}/src/service
        ${CMAKE_SOURCE_DIR}/src/ui
        ${CMAKE_SOURCE_DIR}/src/ui/account
        ${CMAKE_SOURCE_DIR}/src/ui/phone
        ${CMAKE_SOURCE_DIR}/src/ui/user
        ${CMAKE_SOURCE_DIR}/src/ui/pwd
)

set(PROJECT_SOURCES
        # 1. 主函数
        src/main.cpp
        # 2. 第三方库
        lib/sqlite3.o
        # 3. MODEL 层
        src/model/SystemUser.cpp
        src/model/MobilePhone.cpp
        src/model/Account.cpp
        # 4. DAO 层
        src/dao/InitDatabase.cpp # 初始化 Database
        src/dao/SystemUserDAO.cpp
        src/dao/TelecomOperatorDAO.cpp
        src/dao/MobilePhoneDAO.cpp
        src/dao/CategoryDAO.cpp
        src/dao/AccountDAO.cpp
        # 5. Service 层
        src/service/datetime.cpp
        src/service/Environ.cpp
        src/service/PwdUtils.cpp
        src/service/SystemUserService.cpp
        src/service/TelecomOperatorService.cpp
        src/service/MobilePhoneService.cpp
        src/service/AccountService.cpp
        src/service/PwdGenerator.cpp
        # 6. UI 层
        # (6.0) 资源文件
        res/resource.qrc
        res/icon_zinpass.rc # windows平台pe文件图标
        # (6.1) 常用模块
        src/ui/IsInputValid.cpp # 判断输入的数据是否合法-前端部分辅助验证
        src/ui/AuthDlg.cpp # 验证当前用户身份
        src/ui/WidgetPublicMethod.cpp # 部件公用方法
        # (6.2) 系统登录界面、系统用户注册界面
        src/ui/user/LoginDlg.cpp
        src/ui/user/LoginDlg.ui
        src/ui/user/SignupDlg.cpp
        src/ui/user/SignupDlg.ui
        src/ui/user/PersonalMgrForm.cpp
        src/ui/user/PersonalMgrForm.ui
        src/ui/user/CancelPersonalUserForm.cpp
        src/ui/user/CancelPersonalUserForm.ui
        # (6.3) 主工作台
        src/ui/MainWorkbench.cpp
        src/ui/MainWorkbench.ui
        # (6.4) 主功能菜单
        src/ui/FunctionMenuForm.h
        src/ui/FunctionMenuForm.cpp
        src/ui/FunctionMenuForm.ui
        # (6.5) 管理账号：页面框架、添加和修改账号记录、修改账号所属人
        src/ui/account/AccountMgrForm.h
        src/ui/account/AccountMgrForm.cpp
        src/ui/account/AccountMgrForm.ui
        src/ui/account/PopupFilterWindow.h # 下拉筛选弹窗（查找账号）
        src/ui/account/PopupFilterWindow.cpp
        src/ui/account/ExposedPwdDlg.cpp # 显示明文密码的弹窗
        src/ui/account/ExposedPwdDlg.ui
        src/ui/account/AccountDlg.ui # 增改账号记录共用的 ui 类
        src/ui/account/AccountAddDlg.cpp
        src/ui/account/AccountEditDlg.cpp
        src/ui/account/DataOwnerForm.h
        src/ui/account/DataOwnerForm.cpp
        src/ui/account/DataOwnerForm.ui
        # (6.6) 管理手机号：页面框架、增加手机号
        src/ui/phone/PhoneMgrForm.h
        src/ui/phone/PhoneMgrForm.cpp
        src/ui/phone/PhoneMgrForm.ui
        src/ui/phone/PhoneAddDlg.h
        src/ui/phone/PhoneAddDlg.cpp
        src/ui/phone/PhoneAddDlg.ui
        # (6.7) 密码相关页面：随机密码生成器、密码安全检查器
        src/ui/pwd/PwdGenDlg.h
        src/ui/pwd/PwdGenDlg.cpp
        src/ui/pwd/PwdGeneratorDlg.h
        src/ui/pwd/PwdGeneratorDlg.cpp
        src/ui/pwd/CheckPwdSecurityForm.h
        src/ui/pwd/CheckPwdSecurityForm.cpp
        src/ui/pwd/CheckPwdSecurityForm.ui
        # (6.8) 迁移备份数据
        src/ui/OutputForm.h
        src/ui/OutputForm.cpp
        src/ui/OutputForm.ui
        src/ui/InputForm.h
        src/ui/InputForm.cpp
        src/ui/InputForm.ui
)

# 使用 Qt6 的 qt_add_executable
qt_add_executable(ZinPass
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
)

# 链接 Qt6 和 OpenSSL、Windows 系统库
target_link_libraries(ZinPass PRIVATE Qt6::Widgets ${OPENSSL_LIBRARIES} ws2_32 user32 advapi32 crypt32)

# 设置 macOS 相关属性
set_target_properties(ZinPass PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin # 设置可执行文件的输出路径
)

# 安装配置
include(GNUInstallDirs)
install(TARGETS ZinPass
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 最终化 Qt6 可执行文件
qt_finalize_executable(ZinPass)
