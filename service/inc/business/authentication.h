#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>
#include <optional>
#include "models/system_user.h"

namespace zinpass::business {

typedef struct LoginSuccessData {
    models::SystemUser user;
    std::string session_id;
} login_data_t;

template<typename T>
struct AuthResult {
    T data;                        // 登录成功返回的用户数据
    std::string message;           // 消息
};

class Authentication {
public:
    /**
     * @brief 用户使用 id 登录
     * @param id 用户账号的主键
     * @param password 用户密码
     * @return ((SystemUser + session_id) | null opt) + message
     */
    static AuthResult<std::optional<login_data_t>> loginById(const std::string& id, const std::string& password);

    /**
     * @brief 用户使用 username 登录
     * @param username 用户账号
     * @param password 用户密码
     * @return ((SystemUser + session_id) | null opt) + message
     */
    static AuthResult<std::optional<login_data_t>> loginByUsername(const std::string& username, const std::string& password);

    /**
     * @brief 通过 主键 id + password 验证用户合法性
     * @param id 用户账号的主键
     * @param password 用户密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    static AuthResult<bool> validateUserById(const std::string& id, const std::string& password);

    /**
     * @brief 通过 username + password 验证用户合法性
     * @param username 用户账号的用户名
     * @param password 用户密码
     * @return 返回true代表验证成功，返回false代表验证失败
     */
    static AuthResult<bool> validateUserByUsername(const std::string& username, const std::string& password);

    /**
     * @brief 修改系统用户的主密码
     * @param username 用户账号
     * @param old_password 旧密码
     * @param new_password 新密码
     * @return 成功与否
     */
    static AuthResult<bool> changeMainPassword(const std::string& username, const std::string& old_password,const std::string& new_password);
};

}

#endif //AUTHENTICATION_H
