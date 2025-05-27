/**
 * user 中的 id 是实际数据库中的主键，不可变
 * user 中的 username 仅用于用户登录，便于用户记忆和使用的，用户可修改，
 * 除此之外，尽量避免使用 username
 */

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <string>
#include <vector>
#include <optional>
#include "system_user.h"


namespace zinpass::business{

// 定义返回类型
template<typename T>
struct UserResult {
    T data;               // 数据
    std::string message;  // 消息
};

class UserManager {
public:
    /**
     * @brief ListUsernames 获取所有用户的ID
     * @return 用户名列表 + 成功与否 + 消息
     */
    static UserResult<std::vector<std::string>> ListUsernames();

    /**
     * @brief 通过 主键 id 查询用户信息
     * @param id 用户账号的主键
     * @return (查询到的用户数据 | null opt) + 成功与否 + 消息
     */
    static UserResult<std::optional<models::SystemUser>> findById(const std::string& id);

    /**
     * @brief 通过 username 查询用户信息
     * @param username 用户账号的主键
     * @return (查询到的用户数据 | null opt) + 成功与否 + 消息
     */
    static UserResult<std::optional<models::SystemUser>> findByUsername(const std::string& username);

    /**
     * @brief 注册新用户
     * @param username 用户账号登录名
     * @param nickname 昵称
     * @param plaintext_password 用户明文密码
     * @return user_id (UUID)
     */
    static UserResult<std::string> registerSysUser(const std::string& username, const std::string& nickname, const std::string& plaintext_password);

    /**
     * @brief 删除用户（抹除数据而非仅逻辑标记）
     * @param id 目标用户的主键 id
     * @return 删除用户成功与否
     */
    static UserResult<bool> removeSysUser(const std::string& id);

    /**
     * @brief 更新系统用户信息
     * @param id 更新目标
     * @param nickname 用户姓名
     * @return 成功与否
     */
    static UserResult<bool> updateSysUserById(const std::string& id, const std::string& nickname);

    /**
     * @brief 更新系统用户信息
     * @param username 更新目标（username版）
     * @param nickname 用户姓名
     * @return 成功与否
     */
    static UserResult<bool> updateSysUserByUsername(const std::string& username, const std::string& nickname);

};

}
#endif