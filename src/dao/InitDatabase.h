#ifndef INITDATABASE_H
#define INITDATABASE_H

#include <sqlite3.h>
#include <string>

class InitDatabase{
public:
    /**
     * @brief InitDatabase 初始化DB构造函数
     * @param newDbPath 数据库文件路径
     */
    InitDatabase(const std::string& newDbPath);

    /**
     * @brief init 执行初始化操作
     * @return 返回 0 代表成功
     */
    [[nodiscard]] int init() const;
private:
    std::string dbPath;
    static int executeSQL(sqlite3* db, const std::string& sql);
};

#endif // INITDATABASE_H
