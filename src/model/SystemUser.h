#ifndef SYSTEMUSER_H
#define SYSTEMUSER_H

#include <string>

class SystemUser {
private:
    std::string userId;
    std::string alias;
    std::string pwd;
    std::string salt;
    std::string createdTime;
    std::string updatedTime;

public:
    // 默认构造函数
    SystemUser();

    // 带参数的构造函数，参数使用 const 引用，可接受临时对象
    SystemUser(std::string userId, std::string alias, std::string pwd,
               std::string salt, std::string createdTime, std::string updatedTime);

    // 拷贝构造函数，进行深拷贝
    SystemUser(const SystemUser& systemUser);

    // 获取用户 ID 的成员函数
    [[nodiscard]] std::string getUserId() const;

    // 获取用户别名的成员函数
    [[nodiscard]] std::string getAlias() const;

    // 获取哈希密码的成员函数
    [[nodiscard]] std::string getPwd() const;

    // 获取盐值的成员函数
    [[nodiscard]] std::string getSalt() const;

    [[nodiscard]] std::string getCreatedTime() const;

    [[nodiscard]] std::string getUpdatedTime() const;

    // 设置用户 ID 的成员函数，参数使用 const 引用
    void setUserId(const std::string& userId);

    // 设置用户别名的成员函数，参数使用 const 引用
    void setAlias(const std::string& alias);

    // 设置哈希密码的成员函数，参数使用 const 引用
    void setPwd(const std::string& pwd);

    // 设置盐值的成员函数，参数使用 const 引用
    void setSalt(const std::string& salt);

    void setCreatedTime(const std::string& createdTime);

    void setUpdatedTime(const std::string& updatedTime);

    void debug();
};

#endif
