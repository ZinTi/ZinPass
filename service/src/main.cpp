#include <string>
#include <thread>
#include <chrono>
#include <filesystem>
#include "read_configuration_file.h"
#include "init_database.h"
#include "log_manager.h"
#include "state_manager.h"
#include "session_manager.h"
#include "connection_pool.h"
#include "grpc_client_service.h"
#include "grpc_control_service.h"

#define GRPC_CLIENT_SVC_AP      "0.0.0.0:50051"
#define GRPC_CTRL_SVC_AP        "127.0.0.1:50052"

using namespace zinpass;

void init();    // 初始化
[[noreturn]] void periodic_task_worker();  // 定时线程

#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
)
#else
int main()
#endif
{
    utils::LogManager::AddLog("Main thread - Service/Daemon started");
    std::atomic<bool> is_running(true);     // 局部原子控制标志
    init();

    // 创建两个 gRPC 服务实例
    service::gRPCControlService control_interface(is_running, GRPC_CTRL_SVC_AP);                        // gRPC 实例1 - 管理控制
    service::gRPCClientService business_server(is_running, config::StateManager::instance().get_server_address());  // gRPC 实例1 - 业务逻辑

    // 管理控制交互线程：处理管理员命令
    std::thread thread_control_interface([&control_interface]() {
        control_interface.run();
    });

    // 业务服务线程：处理客户端请求
    std::thread thread_business_server([&business_server]() {
        business_server.run();
    });

    // 周期性清理线程：监控运行状态、定期处理过期会话、检查运行环境等
    std::thread thread_periodic_task_worker(periodic_task_worker);

    // 等待所有线程结束
    thread_periodic_task_worker.detach();
    thread_control_interface.join();
    thread_business_server.join();

    utils::LogManager::AddLog("Main thread - All workers completed");            // 无需手动调用Stop，析构函数会自动处理
    return 0;
}

void init() {
    // 1. 读取配置文件
    config::ReadConfigurationFile* config = new config::ReadConfigurationFile;
    config->read_config_dbpath_from_file();   // 从配置文件读取数据库配置
    config::StateManager::instance().set_main_database_path(config->get_db_path());  // 保存配置到 StateManager 单实例中
    delete config;

    // 2. 数据库不存在则创建并初始化
    const std::string db_path = config::StateManager::instance().get_main_database_path();
    if (false == std::filesystem::exists(db_path)) {
        utils::LogManager::AddLog("[MESSAGE] Database file not found. Creating and initializing...");
        if (const repository::InitDatabase initDb(db_path); 0 == initDb.init()) {
            utils::LogManager::AddLog("[MESSAGE] Initial database success.");
        }
    }
    // 3. 初始化连接池单例
    repository::ConnectionPool::initialize(db_path, 5);

    // 4. 配置服务器地址和端口
    config::StateManager::instance().set_server_address(GRPC_CLIENT_SVC_AP);
    utils::LogManager::AddLog("[CONFIG] Server address: " + config::StateManager::instance().get_server_address());

}


// 增加线程池
// 改为条件变量方式
[[noreturn]] void periodic_task_worker() {
    constexpr int seconds = 1800;   // 30min 清理一次无效 session
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        // 自动清理无效 session 记录等操作
        const int sum = business::SessionManager::purge_expired_sessions();
        utils::LogManager::AddLog("[MESSAGE] purged expired sessions, sum: " + std::to_string(sum));
    }
}