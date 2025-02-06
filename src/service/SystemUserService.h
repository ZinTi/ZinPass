#ifndef SYSTEMUSERSERVICE_H
#define SYSTEMUSERSERVICE_H

#include <string>
#include <vector>
#include "SystemUser.h"

class SystemUserService {
public:
    /**
     * @brief getSysUserIdOptions 获取所有用户的ID
     * @param msg 反馈信息
     * @param sysUserId 获取到的sysUserId
     * @return 成功与否
     */
    bool getSysUserIdOptions(std::string& msg, std::vector<std::string>& sysUserId);

    /**
     * @brief 验证用户合法性
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param password 用户密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    bool validateUser(std::string& msg, const std::string& userId, const std::string& password);

    /**
     * @brief 验证当前用户的合法性（二次认证）
     * @param msg 反馈信息
     * @param password 当前用户的密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    bool validateCurrentUser(std::string& msg, const std::string& password);

    /**
     * @brief 用户登录
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param password 用户密码
     * @param systemUser 验证成功的用户信息
     * @return 成功与否
     */
    bool login(std::string& msg, const std::string& userId, const std::string& password, SystemUser& systemUser);

    /**
     * @brief 修改系统用户的密码
     * @param msg 反馈信息
     * @param userId 用户账号
     * @param oldPassword 旧密码
     * @param newPassword 新密码
     * @return 成功与否
     */
    bool changeSysPwd(std::string& msg, const std::string& userId, const std::string& oldPassword,
                             const std::string& newPassword);

    /**
     * @brief 注册新用户
     * @param msg 反馈信息
     * @param user 注册新用户的信息
     * @return 返回 true 代表注册成功， false 代表注册失败
     */
    bool registerSysUser(std::string& msg, SystemUser& user);

    /**
     * @brief 删除用户（删除数据而非逻辑标记）
     * @param msg 反馈信息
     * @param userId 目标用户的userId
     * @return 删除用户成功与否
     */
    bool removeSysUser(std::string& msg, const std::string& userId);

    /**
     * @brief 更新系统用户信息
     * @param msg 反馈信息
     * @param userId 更新目标的用户Id
     * @param alias 用户姓名
     * @return 成功与否
     */
    bool updateSysUser(std::string& msg, const std::string& userId, const std::string& alias);

};
#endif