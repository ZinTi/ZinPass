#include "authentication.h"
#include "common/dao_status.h"
#include "system_user_dao.h"
#include "user_manager.h"
#include "session_manager.h"
#include "pwd_utils.h"
#include "date_time.h"

namespace zinpass::business {
    AuthResult<std::optional<login_data_t>> Authentication::loginById(const std::string& id, const std::string& password) {
        // 0\ 创建部分变量并验证参数合法性

        // 1\ 查询用户信息并验证用户身份合法性
        const auto [data, message] = UserManager::findById(id);
        if (!data) { // 查询用户失败
            return {std::nullopt, message};
        }

        if(const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {std::nullopt, "用户或密码不正确"};
            }

        // 2\ 创建会话
        const std::string session_id = SessionManager::create_session(data->getId(), 1800);
        if (session_id.empty()) {
            return {std::nullopt, "会话建立失败"};
        }

        // 3\ 返回用户数据和会话ID
        login_data_t success_data;
        success_data.user = *data;    // 解引用并使用重载赋值运算符复制给左值
        success_data.session_id = session_id;
        return {success_data, "登录成功"};
    }

    AuthResult<std::optional<login_data_t>> Authentication::loginByUsername(const std::string& username, const std::string& password) {
        // 0\ 创建部分变量并验证参数合法性

        // 1\ 查询用户信息并验证用户身份合法性
        const auto [data, message] = business::UserManager::findByUsername(username);
        if (!data) { // 查询用户失败
            return {std::nullopt, message};
        }

        if(const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {std::nullopt, "用户或密码不正确"};
        }

        // 2\ 创建会话
        const std::string session_id = SessionManager::create_session(data->getId(), 1800);
        if (session_id.empty()) {
            return {std::nullopt, "会话建立失败"};
        }

        // 3\ 返回用户数据和会话ID
        login_data_t success_data;
        success_data.user = *data;    // 解引用并使用重载赋值运算符复制给左值
        success_data.session_id = session_id;
        return {success_data, "登录成功"};
    }

    AuthResult<bool> Authentication::validateUserById(const std::string& id, const std::string& password) {
        // 0\ 创建部分变量并验证参数合法性

        // 1\ 查询用户
        const auto [data, message] = business::UserManager::findById(id);
        if (!data) {
            return {false, message};    // 获取用户信息失败
        }

        if (const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {false, "用户或密码不正确"};
        }
        return {true, "验证成功"};
    }

    AuthResult<bool> Authentication::validateUserByUsername(const std::string& username, const std::string& password) {
        // 0\ 创建部分变量并验证参数合法性

        // 1\ 查询用户
        const auto [data, message] = UserManager::findByUsername(username);
        if (!data) {
            return {false, message};    // 获取用户信息失败
        }

        if (const std::string hashed_password = utils::PwdUtils::digestPassword(password, data->getSalt());
            hashed_password != data->getPassword()) {
            return {false, "用户名或密码不正确"};
            }
        return {true, "验证成功"};
    }


    AuthResult<bool> Authentication::changeMainPassword(const std::string& username, const std::string& old_password, const std::string& new_password) {
        // 0\ 创建部分变量或验证参数合法性

        // 1\ 查询用户信息并验证用户身份
        const auto [data, message] = business::UserManager::findByUsername(username);
        if (!data) {
            return {false, message};    // 获取用户信息失败
        }

        if (const std::string hashed_old_password = utils::PwdUtils::digestPassword(old_password, data->getSalt());
            hashed_old_password != data->getPassword()) {
            return {false, "用户名或密码不正确"};
            }

        // 2\ 生成新密码散列值、新盐值、更新时间
        const std::string new_salt = utils::PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
        const std::string hashed_new_password = utils::PwdUtils::digestPassword(new_password, new_salt); // 根据新盐值生成一个新密码密文
        const std::string now = utils::DateTime::getCurrentDateTime();

        models::SystemUser new_user = *data;
        new_user.setSalt(new_salt);
        new_user.setPassword(hashed_new_password);
        new_user.setUpdatedTime(now);

        repository::SystemUserDAO system_user_dao;
        if (const repository::DaoStatus ret_status = system_user_dao.update(new_user);
            repository::DaoStatus::Success != ret_status) {
            return {false, "系统错误，修改密码失败"};
        }

        // 3\ 更新所有 account 记录的密钥


        // 4\ 如果密钥更新失败，回滚，并撤销主密码变更

        // 5\ 返回结果
        return {true, "修改密码成功"};
    }
}