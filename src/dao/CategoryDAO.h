#ifndef CATEGORYDAO_H
#define CATEGORYDAO_H

#include <string>
#include <vector>

#include "sqlite3.h"

class CategoryDAO {
public:
    explicit CategoryDAO(const std::string& dbPath);
    ~CategoryDAO();

    /**
     * @brief categoryNameToId 将 categoryName 转换成对应的 id 主键
     * @param msg 反馈信息
     * @param categoryName 手机号
     * @param id 查询到的id主键，如果未查询到则 id 为 -1
     * @return 成功与否
     */
    bool categoryNameToId(std::string& msg, const std::string& categoryName, short* id);

    /**
     * @brief 查询所有类别
     * @param msg 反馈信息
     * @param categories 类别
     * @return 成功与否
     */
    bool findAllCategories(std::string& msg, std::vector<std::string>& categories);

private:
    std::string dbPath;  // 数据库路径
    sqlite3* db; // 数据库

    void openDatabase(std::string& msg);  // 打开数据库
    void closeDatabase() const;  // 关闭数据库

};

#endif


