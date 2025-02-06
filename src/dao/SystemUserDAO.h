#ifndef SYSTEMUSERDAO_H
#define SYSTEMUSERDAO_H

#include "SystemUser.h"
#include <string>
#include <vector>
#include "sqlite3.h"

class SystemUserDAO {
public:
    // 构造函数接收数据库路径
    explicit SystemUserDAO(const std::string& dbPath);
    ~SystemUserDAO();

    /**
     * @brief 通过userId查询SystemUser表中记录
     * @param msg 反馈信息
     * @param userId 用户ID
     * @param sysUser 存放查询到的用户信息的容器
     * @return 成功与否
     */
    bool findByUserId(std::string& msg, const std::string& userId, SystemUser& sysUser);

    /**
     * @brief 获取所有userId
     * @param msg 反馈信息
     * @param userIdList 存放所有查询到的userId的容器
     * @return 成功与否
     */
    bool findAllUserId(std::string& msg, std::vector<std::string>& userIdList);

    /**
     * @brief 新增SystemUser记录
     * @param msg 反馈信息
     * @param sysUser 新SystemUser记录
     * @return 成功与否
     */
    bool add(std::string& msg, const SystemUser& sysUser);

    /**
     * @brief 修改SystemUser记录
     * @param msg 反馈信息
     * @param sysUser 新SystemUser记录
     * @return 成功与否
     */
    bool update(std::string& msg, const SystemUser& sysUser);

    /**
     * @brief 删除SystemUser记录
     * @param msg 反馈信息
     * @param userId 删除目标的userId
     * @return 成功与否
     */
    bool remove(std::string& msg, const std::string& userId);

private:
    std::string dbPath; // 数据库路径
    sqlite3* db;
    void openDatabase(std::string& msg); // 打开数据库
    void closeDatabase() const; // 关闭数据库
};

#endif
