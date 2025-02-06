#include "Environ.h"
#include <string>
#include <filesystem>
#include <QCoreApplication>
#include <QDir>
#include "InitDatabase.h"

Environ::Environ() {
    if (std::string msg; setHomePath(msg)) {
        // 设置db路径
        setDbPath();
        // 若db不存在，则创建DB文件
        if(false == std::filesystem::exists(std::filesystem::path(dbPath))){
            const InitDatabase initDb(dbPath);
            if (0 == initDb.init()) {
                ;// 成功
            }
        }
        //
    }
    else {
        exit(-1);
    }
}

std::string Environ::getHomePath() {
    return homePath;
}

std::string Environ::getDbPath() {
    return dbPath;
}

// 设置home路径函数，返回是否成功设置
bool Environ::setHomePath(std::string& msg) {
    // 使用 Qt 的 QCoreApplication 类获取应用程序的目录路径
    const QString appPath = QCoreApplication::applicationDirPath();
    // 使用 QDir 获取父目录
    QDir dir(appPath);
    if (dir.cdUp()) {
        // 将 QString 转换为 std::string
        homePath = dir.absolutePath().toStdString();
        // 假设获取成功
        msg = "Success";
        return true;
    } else {
        msg = "Failed to get parent directory";
        return false;
    }
}

void Environ::setDbPath() {
    // 拼接数据库路径
    dbPath = homePath + "/data/zinpass.db";
    // 将结果转换为 std::string 存储在 dbPath 中
}
