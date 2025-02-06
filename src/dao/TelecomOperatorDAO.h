#ifndef TELECOMOPERATORDAO_H
#define TELECOMOPERATORDAO_H

#include <string>
#include <vector>
#include "sqlite3.h"

class TelecomOperatorDAO {
public:
    explicit TelecomOperatorDAO(const std::string& dbPath);
    ~TelecomOperatorDAO();
    bool findList(std::string& msg, std::vector<std::string>& telecomOperators);

private:
    std::string dbPath;  // 数据库路径
    sqlite3* db; // 数据库

    void openDatabase(std::string& msg);  // 打开数据库
    void closeDatabase() const;  // 关闭数据库
};

#endif //TELECOMOPERATORDAO_H
