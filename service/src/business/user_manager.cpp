#include "user_manager.h"
#include "system_user_dao.h"
#include "pwd_utils.h"
#include "date_time.h"
#include "common/dao_status.h"
#include "uuid_generator.h"

namespace zinpass::business {

UserResult<std::vector<std::string>> UserManager::ListUsernames() {
    repository::SystemUserDAO system_user_dao;
    std::string message;
    std::vector<std::string> sys_usernames;
    if (const repository::DaoStatus ret_status = system_user_dao.findUsernames(sys_usernames);
        repository::DaoStatus::Success != ret_status ) {
        message = "发生错误";
    } else {
        message = "成功";
    }
    return {sys_usernames, message};
}

UserResult<std::optional<models::SystemUser>> UserManager::findById(const std::string& id) {
    repository::SystemUserDAO system_user_dao;
    models::SystemUser system_user;
    if (const repository::DaoStatus ret_status = system_user_dao.findById(id, system_user);
        repository::DaoStatus::Success != ret_status) {
        return {std::nullopt, "发生错误"};
    }
    return {system_user, "成功"};
}

UserResult<std::optional<models::SystemUser>> UserManager::findByUsername(const std::string& username) {
    repository::SystemUserDAO system_user_dao;
    models::SystemUser system_user;
    if (const repository::DaoStatus ret_status = system_user_dao.findByUsername(username, system_user);
        repository::DaoStatus::Success != ret_status) {
        return {std::nullopt,  "发生错误"};
    }
    return {system_user,  "成功"};
}

UserResult<std::string> UserManager::registerSysUser(const std::string& username, const std::string& nickname, const std::string& plaintext_password) {
    // 0\ 参数数据合法性校验
    if (username.length() < 3 || username.length() > 16) {
        return {"", "用户名长度不合法"};
    }
    if (plaintext_password.length() < 6 || plaintext_password.length() > 36) {
        return {"", "密码长度不合法"};
    }

    // 1\ 构造用户数据对象
    const std::string new_salt = utils::PwdUtils::generateRandomString();  // 新生成一个 salt
    const std::string hashed_password = utils::PwdUtils::digestPassword(plaintext_password, new_salt); // 根据密码和盐值生成一个密码散列值
    if (hashed_password.empty()) { // 为空即失败
        return {"", "故障，请稍后再试"}; // 散列失败
    }
    const std::string now = utils::DateTime::getCurrentDateTime();
    const std::string user_id = utils::UUIDGenerator::generate();
    const models::SystemUser new_user(
        user_id,        // id 主键
        username,       // username 用户名
        nickname,       // nickname 昵称
        hashed_password,// password 密码散列值
        new_salt,       // salt 盐值
        now,            // 创建时间
        now             // 更新时间
        );

    // 2\ 写入
    repository::SystemUserDAO system_user_dao;
    if (const repository::DaoStatus ret_status = system_user_dao.add(new_user);
        repository::DaoStatus::Success != ret_status) {
        return {"", "创建用户失败"};
    }
    return {user_id, "成功"};
}

UserResult<bool> UserManager::removeSysUser(const std::string& id) {
    repository::SystemUserDAO system_user_dao;
    if (const repository::DaoStatus ret_status = system_user_dao.remove(id);
        repository::DaoStatus::Success != ret_status) {
        return {false, std::string("失败：") + std::to_string(static_cast<int>(ret_status))};
    }
    return {true, std::string("成功删除用户") + id};
}

UserResult<bool> UserManager::updateSysUserById(const std::string& id, const std::string& nickname) {
    repository::SystemUserDAO system_user_dao;

    models::SystemUser system_user;

    // 2\ 获取原来的值
    if (const repository::DaoStatus ret_status_1 = system_user_dao.findById(id, system_user);
        repository::DaoStatus::Success != ret_status_1) {
        return {false, "获取原值失败"};
    }

    // 3\ 更新新值
    system_user.setNickname(nickname);
    if (const repository::DaoStatus ret_status_2 = system_user_dao.update(system_user);
        repository::DaoStatus::Success != ret_status_2) {
        return {false, "更新新值失败"};
    }

    return {true, "昵称更新成功"};
}

UserResult<bool> UserManager::updateSysUserByUsername(const std::string& username, const std::string& nickname) {
    repository::SystemUserDAO system_user_dao;
    models::SystemUser system_user;

    // 2\ 获取原来的值
    if (const repository::DaoStatus ret_status_1 = system_user_dao.findByUsername(username, system_user);
        repository::DaoStatus::Success != ret_status_1) {
        return {false, "获取原值失败"};
        }

    // 3\ 更新新值
    system_user.setNickname(nickname);
    if (const repository::DaoStatus ret_status_2 = system_user_dao.update(system_user);
        repository::DaoStatus::Success != ret_status_2) {
        return {false, "更新新值失败"};
        }

    return {true, "昵称更新成功"};
}

}