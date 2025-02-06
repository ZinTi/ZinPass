#include "SystemUser.h"
#include <utility>
#include <iostream>

// 默认构造函数
SystemUser::SystemUser() {
    userId = "";
    alias = "";
    pwd = "";
    salt = "";
    createdTime = "";
    updatedTime = "";
}

// 带参数的构造函数，参数使用 const 引用，可接受临时对象
SystemUser::SystemUser(std::string userId, std::string alias, std::string pwd,std::string salt, std::string createdTime, std::string updatedTime)
    : userId(std::move(userId)), alias(std::move(alias)), pwd(std::move(pwd)),
      salt(std::move(salt)), createdTime(std::move(createdTime)),updatedTime(std::move(updatedTime)) {
}

// 拷贝构造函数，进行深拷贝
SystemUser::SystemUser(const SystemUser& systemUser) {
    userId = systemUser.userId;
    alias = systemUser.alias;
    pwd = systemUser.pwd;
    salt = systemUser.salt;
    createdTime = systemUser.createdTime;
    updatedTime = systemUser.updatedTime;
}

// 获取用户 ID 的成员函数
std::string SystemUser::getUserId() const {
    return userId;
}

// 获取用户别名的成员函数
std::string SystemUser::getAlias() const {
    return alias;
}

// 获取哈希密码的成员函数
std::string SystemUser::getPwd() const {
    return pwd;
}

// 获取盐值的成员函数
std::string SystemUser::getSalt() const {
    return salt;
}

std::string SystemUser::getCreatedTime() const {
    return createdTime;
}

std::string SystemUser::getUpdatedTime() const {
    return updatedTime;
}

// 设置用户 ID 的成员函数，参数使用 const 引用
void SystemUser::setUserId(const std::string& userId) {
    this->userId = userId;
}

// 设置用户别名的成员函数，参数使用 const 引用
void SystemUser::setAlias(const std::string& alias) {
    this->alias = alias;
}

// 设置哈希密码的成员函数，参数使用 const 引用
void SystemUser::setPwd(const std::string& pwd) {
    this->pwd = pwd;
}

// 设置盐值的成员函数，参数使用 const 引用
void SystemUser::setSalt(const std::string& salt) {
    this->salt = salt;
}

void SystemUser::setCreatedTime(const std::string& createdTime) {
    this->createdTime = createdTime;
}

void SystemUser::setUpdatedTime(const std::string& updatedTime) {
    this->updatedTime = updatedTime;
}

void::SystemUser::debug(){
    std::cout << "[debug - SystemUser]" << std::endl;
    std::cout << "userId: " << this->userId << std::endl;
    std::cout << "alias: " << this->alias << std::endl;
    std::cout << "pwd: " << this->pwd << std::endl;
    std::cout << "salt: " << this->salt << std::endl;
    std::cout << "createdTime: " << this->createdTime << std::endl;
    std::cout << "updatedTime: " << this->updatedTime << std::endl;
}
