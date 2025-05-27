#include "read_configuration_file.h"
#include <fstream>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

namespace zinpass::config {

    ReadConfigurationFile::ReadConfigurationFile() {
        this->database_path = "";
    }
    ReadConfigurationFile::~ReadConfigurationFile() {

    }

    void ReadConfigurationFile::read_config_dbpath_from_file() {
        // 获取可执行文件路径
        std::string exe_dir;
        char buffer[1024] = {0};

#ifdef _WIN32
        HMODULE hModule = GetModuleHandle(nullptr);
        GetModuleFileNameA(hModule, buffer, sizeof(buffer));
        std::string exe_path(buffer);
        size_t pos = exe_path.find_last_of("\\/");
#else
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            std::string exe_path(buffer);
            size_t pos = exe_path.find_last_of('/');
#endif

        if (pos != std::string::npos) {
            exe_dir = exe_path.substr(0, pos);
        }
        else {
            exe_dir = ".";
        }

#ifndef _WIN32
    }
#endif

    // 构建配置文件路径
    std::string config_path = exe_dir + "/zinpass.config";

    // 创建文件（如果不存在）
    std::ifstream test_file(config_path);
    if (!test_file) {
        std::ofstream create_file(config_path);
        if (create_file)
            create_file.close();
    }

    // 读取配置文件
    bool found = false;
    std::ifstream config_file(config_path);
    std::string line;

    while (std::getline(config_file, line)) {
        // 去除前后空格
        auto trim = [](std::string& s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
            s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(), s.end());
        };

        trim(line);
        if (line.empty())
            continue;

        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos)
            continue;

        std::string key = line.substr(0, eq_pos);
        trim(key);
        if (key != "database")
            continue;

        std::string value = line.substr(eq_pos + 1);
        trim(value);

        // 解析带引号的值
        if (!value.empty()) {
            char quote = value.front();
            if (quote == '"' || quote == '\'') {
                size_t end_quote = value.find(quote, 1);
                if (end_quote != std::string::npos) {
                    database_path = value.substr(1, end_quote - 1);
                    found = true;
                    break;
                }
            }
            else {
                database_path = value;
                found = true;
                break;
            }
        }
    }

    // 设置默认路径
    if (!found) {
        database_path = exe_dir + "/zinpass.db";
    }

    // 统一路径分隔符（可选）
#ifdef _WIN32
    std::replace(database_path.begin(), database_path.end(), '/', '\\');
#else
            std::replace(database_path.begin(), database_path.end(), '\\', '/');
#endif
}

std::string ReadConfigurationFile::get_db_path(){
    return database_path;
}


}

