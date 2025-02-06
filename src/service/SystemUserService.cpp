#include "SystemUserService.h"
#include "Environ.h"
#include "SystemUserDAO.h"
#include "PwdUtils.h"
#include "datetime.h"

extern Environ* ZinPassEnv;
extern SystemUser* currentSystemUser;

bool SystemUserService::getSysUserIdOptions(std::string& msg, std::vector<std::string>& sysUserId) {
    bool result;
    if (
        SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());
        systemUserDAO.findAllUserId(msg, sysUserId)
    ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool SystemUserService::validateUser(std::string& msg, const std::string& userId, const std::string& password) {
    bool result; // 返回值
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());

    SystemUser systemUser;
    if (systemUserDAO.findByUserId(msg, userId, systemUser)) { // 找到信息
        const std::string hashedPassword = PwdUtils::digestPassword(password, systemUser.getSalt());
        if (hashedPassword == systemUser.getPwd()) {
            msg.append(";验证通过;");
            result = true;
        }
        else {
            msg.append(";验证失败;");
            result = false;
        }
    }
    else {
        msg.append(";验证失败;");
        result = false;
    }

    return result;
}


bool SystemUserService::validateCurrentUser(std::string& msg, const std::string& password) {
    bool result; // 返回值
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());

    SystemUser systemUser;
    if (systemUserDAO.findByUserId(msg, currentSystemUser->getUserId(), systemUser)) { // 找到信息
        const std::string hashedPassword = PwdUtils::digestPassword(password, systemUser.getSalt());
        if (hashedPassword == systemUser.getPwd()) {
            msg.append(";验证通过;");
            result = true;
        }
        else {
            msg.append(";验证失败;");
            result = false;
        }
    }
    else {
        msg.append(";验证失败;");
        result = false;
    }

    return result;
}

bool SystemUserService::login(std::string& msg, const std::string& userId, const std::string& password,
                        SystemUser& systemUser) {
    bool result;
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());

    if (systemUserDAO.findByUserId(msg, userId, systemUser)) { // 找到信息
        const std::string hashedPassword = PwdUtils::digestPassword(password, systemUser.getSalt());
        if (hashedPassword == systemUser.getPwd()) {
            msg.append(";验证通过;");
            result = true;
        }
        else {
            msg.append(";验证失败;");
            result = false;
        }
    }
    else {
        msg.append(";验证失败;");
        result = false;
    }
    return result;
}

bool SystemUserService::changeSysPwd(std::string& msg, const std::string& userId, const std::string& oldPassword,
                               const std::string& newPassword) {
    bool result;
    if (!validateUser(msg, userId, oldPassword)) {
        msg = "合法性校验失败！请检查账号和密码";
        result = false;
    }
    else {
        SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());
        SystemUser systemUser;
        systemUserDAO.findByUserId(msg, userId, systemUser); // 获取旧信息
        // 更新新盐值、新密码、更新时间
        const std::string newSalt = PwdUtils::generateRandomString(); // 设置密码时生成一个新盐值
        const std::string newHashedPwd = PwdUtils::digestPassword(newPassword, newSalt); // 根据新盐值生成一个新密码密文
        systemUser.setSalt(newSalt);
        systemUser.setPwd(newHashedPwd);
        systemUser.setUpdatedTime(getCurrentDateTime());

        if (!systemUserDAO.update(msg, systemUser)) {
            msg = "错误！操作数据库时密码更新失败";
            result = false;
        }
        else {
            msg = "成功！已修改密码";
            result = true;
        }
    }

    return result;
}

bool SystemUserService::registerSysUser(std::string& msg, SystemUser& user) {
    bool result; // 返回值
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());

    user.setSalt(PwdUtils::generateRandomString()); // 新生成一个salt
    const std::string newHashedPwd = PwdUtils::digestPassword(user.getPwd(), user.getSalt());
    // 根据盐值生成一个新密码密文
    if (!newHashedPwd.empty()) { // 不为空即代表成功生成新密文
        user.setPwd(newHashedPwd);
    }
    const std::string now = getCurrentDateTime();
    user.setCreatedTime(now);
    user.setUpdatedTime(now);
    //user.debug();
    result = systemUserDAO.add(msg, user);
    return result;
}

bool SystemUserService::removeSysUser(std::string& msg, const std::string& userId) {
    bool result;
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());
    result = systemUserDAO.remove(msg, userId);
    return result;
}

bool SystemUserService::updateSysUser(std::string& msg, const std::string& userId, const std::string& alias) {
    bool result; // 返回值
    SystemUserDAO systemUserDAO(ZinPassEnv->getDbPath());

    SystemUser systemUser;
    if (systemUserDAO.findByUserId(msg, userId, systemUser)) {
        systemUser.setAlias(alias); // 设置修改别名
        result = systemUserDAO.update(msg, systemUser);
    }
    else {
        result = false;
    }

    return result;
}
