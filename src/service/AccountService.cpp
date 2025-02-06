#include "AccountService.h"
#include "Environ.h"
#include "datetime.h"
#include "SystemUser.h"
#include "AccountDAO.h"
#include "MobilePhoneDAO.h"
#include "CategoryDAO.h"
#include "PwdUtils.h"

extern Environ* ZinPassEnv;
extern SystemUser* currentSystemUser;

const std::vector<unsigned char> AccountService::mainPwdToRealKey(const std::string& mainPwd) const{
    const std::string realKeyHex = PwdUtils::calculateMD5(mainPwd);
    const std::vector<unsigned char> realKey = PwdUtils::hexStringToKey(realKeyHex);
    return realKey;
}


bool AccountService::getEmailOptions(std::string& msg, std::vector<std::string>& emailAddress) {
    bool result;
    if (
        AccountDAO emailDAO(ZinPassEnv->getDbPath());
        emailDAO.findEmailList(msg, currentSystemUser->getUserId(), emailAddress)
    ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool AccountService::getCategoryOptions(std::string& msg, std::vector<std::string>& categories) {
    bool result;
    if (
        CategoryDAO categoryDAO(ZinPassEnv->getDbPath());
        categoryDAO.findAllCategories(msg, categories)
    ) {
        result = true;
    }else {
        result = false;
    }
    return result;
}

bool AccountService::getAllAccounts(
    std::string& msg,
    const std::string& providerName,
    const std::string& platformName,
    const std::string& userId,
    const std::string& alias,
    const std::string& tele,
    const std::string& email,
    const std::string& updatedTimeStart,
    const std::string& updatedTimeEnd,
    std::vector<ViewAccount>& viewAccounts
) {
    bool result;
    if (
        AccountDAO accountDAO(ZinPassEnv->getDbPath());
        accountDAO.find(msg, providerName, platformName, userId, alias, tele, email, updatedTimeStart, updatedTimeEnd, currentSystemUser->getUserId(), viewAccounts)
    ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool AccountService::getAccountById(std::string& msg, const int accountId, ViewAccount& viewAccount) {
    bool result;
    AccountDAO accountDAO(ZinPassEnv->getDbPath());
    ViewAccount tempViewAccount;
    if (accountDAO.findById(msg, accountId, tempViewAccount)) {
        if (tempViewAccount.getSysUserId() != currentSystemUser->getUserId()) {
            msg = "无权限";
            result = false;
        }
        else {
            viewAccount.copy(tempViewAccount);
            result = true;
        }
    }
    else {
        result = false;
    }
    return result;
}

bool AccountService::getAccountPwd(std::string& msg, const std::string& mainPwd, const int accountId, std::string& password) {
    bool result;
    std::vector<unsigned char> encryptedPwd, iv;
    std::string sysUserId;
    AccountDAO accountDAO(ZinPassEnv->getDbPath());
    accountDAO.findEncryptedPwdAndIvById(msg, accountId, encryptedPwd, iv, sysUserId);

    if (sysUserId.empty()) {
        msg = "未查询到数据";
        password = "";
        result = false;
    }
    else if (sysUserId != currentSystemUser->getUserId()) { // 过滤攻击
        msg = "没有权限";
        password = "";
        result = false;
    }
    else { // 解密
        if(PwdUtils::decryptTextByAES128CTR(encryptedPwd, iv, mainPwdToRealKey(mainPwd), password)){
            msg = "查询成功！";
            result = true;
        }else{
            msg = "解密失败";
            result = false;
        }
    }

    return result;
}

bool AccountService::addAccount(std::string& msg,
                            const std::string& mainPwd,
                            const std::string& userId,
                            const std::string& alias,
                            const std::string& pwd,
                            const std::string& subAccount,
                            const std::string& phone,
                            const std::string& email,
                            const std::string& postscript,
                            const std::string& platform,
                            const std::string& provider,
                            const std::string& url,
                            const std::string& hotline,
                            const std::string& category
) {
    bool result;
    // 这里对用户输入的数据进行合法性校验（后端部分，核心，前端部分只是辅助，并不安全，后端的数据合法性校验才是最重要的）
    // 如果数据合法则传入account对象中，否则结束
    AccountDAO accountDAO(ZinPassEnv->getDbPath());
    // 对viewAccount 转换成 account
    Account account;
    account.setUserId(userId);
    account.setAlias(alias);

    // 加密处理
    std::vector<unsigned char> newPwd, newIv;

    if(
        PwdUtils::encryptTextByAES128CTR(pwd, mainPwdToRealKey(mainPwd), newPwd, newIv)
        ){
        account.setEncryptedPwd(newPwd);
        account.setIv(newIv);
    }else{
        msg = "新增记录时出现错误！原因是：数据加密失败！";
        return false;
    }

    account.setSubAccount(subAccount);
    MobilePhoneDAO phoneDAO(ZinPassEnv->getDbPath());
    std::string phoneMsg;
    short phoneId;
    if (phone == "无" || phone == "空" || phone == "NULL") {
        account.setPhoneId(-1);
    }
    else {
        if (phoneDAO.phoneNumberToId(phoneMsg, phone, &phoneId)) {
            account.setPhoneId(phoneId);
        }
        else {
            account.setPhoneId(-1);
        }
    }
    AccountDAO emailDAO(ZinPassEnv->getDbPath());
    std::string emailMsg;
    short emailId;
    if (email == "无" || email == "空" || email == "NULL") {
        account.setEmailId(-1);
    }
    else {
        if (emailDAO.emailAddressToId(emailMsg, email, &emailId)) {
            account.setEmailId(emailId);
        }
        else {
            account.setEmailId(-1);
        }
    }

    // account.setId(primaryKey); // 主键
    account.setPostscript(postscript);
    account.setPlatformName(platform);
    account.setProviderName(provider);
    account.setURL(url);
    account.setHotline(hotline);

    CategoryDAO categoryDAO(ZinPassEnv->getDbPath());
    std::string categoryMsg;
    short categoryId;
    if (categoryDAO.categoryNameToId(categoryMsg, category, &categoryId)) {
        account.setCategoryId(categoryId);
    }
    else {
        account.setCategoryId(-1);
    }

    account.setSysUserId(currentSystemUser->getUserId()); // 获取当前用户
    const std::string currentDateTime = getCurrentDateTime(); // 获取当前时间
    account.setCreatedTime(currentDateTime);
    account.setUpdatedTime(currentDateTime);

    // 插入数据
    if (accountDAO.add(msg, account)) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool AccountService::updateAccount(std::string& msg,const std::string& mainPwd,const int accountId,const std::string& userId,const std::string& alias,const std::string& pwd,
                                const std::string& subAccount,const std::string& phone,const std::string& email,const std::string& postscript,
                                const std::string& platform,const std::string& provider,const std::string& url,const std::string& hotline, const std::string& category ) {
    bool result;

    // currentSystemUser->getUserId(); // 获取当前用户，如果目标数据不是当前用户则返回

    std::string currentDateTime = getCurrentDateTime();

    short phoneId, emailId;
    std::string phoneMsg, emailMsg;
    MobilePhoneDAO phoneDAO(ZinPassEnv->getDbPath());
    AccountDAO emailDAO(ZinPassEnv->getDbPath());
    if (false == phoneDAO.phoneNumberToId(phoneMsg, phone, &phoneId)) {
        phoneId = -1;
    }
    if (false == emailDAO.emailAddressToId(emailMsg, email, &emailId)) {
        emailId = -1;
    }

    AccountDAO accountDAO(ZinPassEnv->getDbPath());

    std::vector<unsigned char> encryptedPwd, iv;
    if (pwd.empty()) { // 1\ 如果是空密码，则查询原密码
        std::string pwdMsg;
        std::string getOwner;
        if(
            false == accountDAO.findEncryptedPwdAndIvById(pwdMsg, accountId, encryptedPwd, iv, getOwner)
            ){
            msg = "程序出错";
            return false;
        }
    }else{ // 2\ 否则更新密码
        // 加密
        if(false == PwdUtils::encryptTextByAES128CTR(pwd, mainPwdToRealKey(mainPwd), encryptedPwd, iv)){
            msg = "加密失败";
            return false;
        }
    }

    CategoryDAO categoryDAO(ZinPassEnv->getDbPath());
    std::string categoryMsg;
    short categoryId;
    if (categoryDAO.categoryNameToId(categoryMsg, category, &categoryId)) {
        //
    }
    else {
        categoryId = -1;
    }

    if (
        accountDAO.update(msg, accountId, provider, platform, url, hotline, currentDateTime) &&
        accountDAO.update(msg, accountId, userId, alias, encryptedPwd, iv, categoryId, currentDateTime) &&
        accountDAO.update(msg, accountId, phoneId, emailId, currentDateTime) &&
        accountDAO.update(msg, accountId, subAccount, postscript, currentDateTime)
        ) {
        msg =  "更新成功：" + msg;
        result = true;
    } else {
        msg = "更新失败：" + msg;
        result = false;
    }

    return result;
}
/*
bool AccountService::updateAccount(std::string& msg,const int accountId,const std::string& userId,const std::string& alias,const std::string& pwd,
                                const std::string& subAccount,const std::string& phone,const std::string& email,const std::string& postscript,
                                const std::string& platform,const std::string& provider,const std::string& url,const std::string& hotline ) {
    bool result;

    // currentSystemUser->getUserId(); // 获取当前用户，如果目标数据不是当前用户则返回

    std::vector<AccountDAO::KeyValuePair> datas;
    // 设置需要更新的数据
    //updateData[AccountDAO::ColumnType::id] = accountId;
    datas.push_back({AccountDAO::ColumnType::userId, userId});
    datas.push_back({AccountDAO::ColumnType::alias, alias});
    if (false == pwd.empty()) {
        datas.push_back({AccountDAO::ColumnType::password, pwd});
    }
    datas.push_back({AccountDAO::ColumnType::subAccount, subAccount});
    short phoneId, emailId;
    std::string phoneMsg, emailMsg;
    MobilePhoneDAO phoneDAO(ZinPassEnv->getDbPath());
    AccountDAO emailDAO(ZinPassEnv->getDbPath());
    if (phoneDAO.phoneNumberToId(phoneMsg, phone, &phoneId)) {
        datas.push_back({AccountDAO::ColumnType::phoneId, phoneId});
    }
    if (emailDAO.emailAddressToId(emailMsg, email, &emailId)) {
        datas.push_back({AccountDAO::ColumnType::emailId, emailId});
    }
    datas.push_back({AccountDAO::ColumnType::postscript, postscript});
    datas.push_back({AccountDAO::ColumnType::platformName, platform});
    datas.push_back({AccountDAO::ColumnType::providerName, provider});
    datas.push_back({AccountDAO::ColumnType::URL, url});
    datas.push_back({AccountDAO::ColumnType::hotline, hotline});
    datas.push_back({AccountDAO::ColumnType::owner, phoneId});
    datas.push_back({AccountDAO::ColumnType::updatedTime, getCurrentDateTime()}); // 只要更新“更新时间”，“创建时间”不用改

    //
    AccountDAO accountDAO(ZinPassEnv->getDbPath());
    if (accountDAO.update(msg, datas, accountId)) {
        msg =  "更新成功：" + msg;
        result = true;
    } else {
        msg = "更新失败：" + msg;
        result = false;
    }

    return result;
}*/

bool AccountService::deleteAccount(std::string& msg, const int accountId) {
    bool result;
    AccountDAO accountDAO(ZinPassEnv->getDbPath());
    if (accountDAO.remove(msg, accountId)) {
        msg = "删除成功";
        result = true;
    }else {
        result = false;
    }
    return result;
}
