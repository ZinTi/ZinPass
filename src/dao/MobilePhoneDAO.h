#ifndef MOBILEPHONEDAO_H
#define MOBILEPHONEDAO_H

#include "MobilePhone.h"
#include <string>
#include "sqlite3.h"
#include <vector>

class MobilePhoneDAO {
public:
    explicit MobilePhoneDAO(const std::string& dbPath);
    ~MobilePhoneDAO();

    /**
     * @brief 根据id查询目标mobilePhone记录
     * @param msg 反馈信息
     * @param id id
     * @param mobilePhone 保存查询到的mobilePhone信息
     * @return 成功与否
     */
    bool findById(std::string& msg, int id, MobilePhone& mobilePhone);

    /**
     * @brief 根据phoneNumber查询目标mobilePhone记录
     * @param msg 反馈信息
     * @param phoneNumber 手机号
     * @param mobilePhone 保存查询到的mobilePhone信息
     * @return 成功与否
     */
    bool findByMobilePhone(std::string& msg, const std::string& phoneNumber, MobilePhone& mobilePhone);


    /**
     * @brief phoneNumberToId 将PhoneNumber转换成对应的id主键
     * @param msg 反馈信息
     * @param phoneNumber 手机号
     * @param id 查询到的id主键，如果未查询到则 id 为 -1
     * @return 成功与否
     */
    bool phoneNumberToId(std::string& msg, const std::string& phoneNumber, short* id);

    /**
     * @brief 查询所有mobilePhone记录
     * @param msg 反馈信息
     * @param sysUserId 筛选条件：当前登录用户
     * @param mobilePhones 保存查询到的mobilePhone信息的vector容器
     * @return 成功与否
     */
    bool findAll(std::string& msg, const std::string& sysUserId, std::vector<MobilePhone>& mobilePhones);

    /**
     * @brief 查询所有符合条件的phoneNumber的列表
     * @param msg 反馈信息
     * @param sysUserId 系统账号ID
     * @param phoneNumberList 存放所有查询到的phoneNumber的容器
     * @return 成功与否
     */
    bool findList(std::string& msg, const std::string& sysUserId, std::vector<std::string>& phoneNumberList);


    /**
     * @brief 新增MobilePhone记录
     * @param msg 反馈信息
     * @param mobilePhone 新MobilePhone记录
     * @return 成功与否
     */
    bool add(std::string& msg, const MobilePhone& mobilePhone);

    /**
     * @brief 修改MobilePhone记录（1/3）
     * @param msg 反馈信息
     * @param id 主键
     * @param updatedTime 更新时间
     * @param phoneNumber 手机号
     * @param servicePwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @return 成功与否
     */
    bool update(std::string& msg, short id, const std::string& updatedTime, const std::string& phoneNumber,
                const std::string& servicePwd, const std::string& pin, const std::string& puk);

    /**
     * @brief 修改MobilePhone记录（2/3）
     * @param msg 反馈信息
     * @param id 主键
     * @param updatedTime 更新时间
     * @param telecomOperator 电信运营商
     * @param joinTime 入网时间
     * @param phoneArea 归属地
     * @return 成功与否
     */
    bool update(std::string& msg, short id, const std::string& updatedTime, const std::string& telecomOperator,
                const std::string& joinTime, const std::string& phoneArea);

    /**
     * @brief 修改MobilePhone记录（3/3）
     * @param msg 反馈信息
     * @param id 主键
     * @param updatedTime 更新时间
     * @param owner 所属人
     * @return 成功与否
     */
    bool update(std::string& msg, short id, const std::string& updatedTime, const std::string& owner);

    /**
     * @brief 删除MobilePhone记录
     * @param msg 反馈信息
     * @param id 删除目标的id
     * @return 成功与否
     */
    bool remove(std::string& msg, short id);

private:
    std::string dbPath; // 数据库路径
    sqlite3* db; // 数据库

    void openDatabase(std::string& msg); // 打开数据库
    void closeDatabase() const; // 关闭数据库
    static void stmtToMobilePhone(sqlite3_stmt* stmt, MobilePhone& mobilePhone);

    /**
     * @brief 生成主键
     * @param msg 反馈信息
     * @return 生成的id
     */
    int generateId(std::string& msg);
};

#endif
