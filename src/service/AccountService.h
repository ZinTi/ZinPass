#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H

#include <string>
#include <vector>
#include "Account.h"

class AccountService {
public:
    /**
     * @brief getEmailOptions
     * @param msg 反馈信息
     * @param emailAddress 获取到的邮箱地址
     * @return 成功与否
     */
    bool getEmailOptions(std::string& msg, std::vector<std::string>& emailAddress);

    /**
     * @brief getCategoryOptions
     * @param msg 反馈信息
     * @param categories 类别
     * @return 成功与否
     */
    bool getCategoryOptions(std::string& msg, std::vector<std::string>& categories);

    /**
     * @brief getAllAccounts 获取所有viewAccounts信息
     * @param msg 反馈信息
     * @param providerName 服务商名称
     * @param platformName 平台名称
     * @param userId 账号ID
     * @param alias 别称
     * @param tele 手机号
     * @param email 邮箱
     * @param updatedTimeStart 更新时间左区间
     * @param updatedTimeEnd 更新时间右区间
     * @param viewAccounts 账号
     * @return 成功与否
     */
    bool getAllAccounts(std::string& msg,const std::string& providerName,const std::string& platformName,const std::string& userId,const std::string& alias,const std::string& tele,const std::string& email,const std::string& updatedTimeStart,const std::string& updatedTimeEnd,std::vector<ViewAccount>& viewAccounts);

    /**
     * @brief 根据id主键获取viewAccounts信息
     * @param msg 反馈信息
     * @param accountId 目标
     * @param viewAccount 获取到的viewAccount
     * @return 成功与否
     */
    bool getAccountById(std::string& msg, int accountId, ViewAccount& viewAccount);

    /**
     * @brief 获取账号密码
     * @param msg 反馈信息
     * @param mainPwd 主密码（密钥）
     * @param accountId 账号id
     * @param password 获取到的密码
     * @return 成功与否
     */
    bool getAccountPwd(std::string& msg, const std::string& mainPwd, int accountId, std::string& password);

    /**
     * @brief 当前用户增加account记录
     * @param msg 反馈信息
     * @param mainPwd 主密码（密钥）
     * @param userId 账号
     * @param alias 昵称
     * @param pwd 密码
     * @param subAccount 子账号
     * @param phone 绑定手机号
     * @param email 绑定邮箱
     * @param postscript 备注
     * @param platform 平台名
     * @param provider 服务商
     * @param url 网址
     * @param hotline 客服热线
     * @param category 类别
     * @return 成功与否
     */
    bool addAccount(std::string& msg, const std::string& mainPwd,const std::string& userId,const std::string& alias,const std::string& pwd,const std::string& subAccount,const std::string& phone,const std::string& email,const std::string& postscript,const std::string& platform,const std::string& provider,const std::string& url,const std::string& hotline, const std::string& category);

    /**
     * @brief 更新Account记录
     * @param msg 反馈信息
     * @param mainPwd 主密码
     * @param accountId 主键，更新目标
     * @param userId 账号
     * @param alias 昵称
     * @param pwd 密码
     * @param subAccount 子账号
     * @param phone 手机号
     * @param email 邮箱
     * @param postscript 备注
     * @param platform 平台名
     * @param provider 服务商
     * @param url 邮件地址
     * @param hotline 服务热线
     * @param categoryId 类别
     * @return 成功与否
     */
    bool updateAccount(std::string& msg, const std::string& mainPwd, int accountId, const std::string& userId, const std::string& alias, const std::string& pwd,const std::string& subAccount,const std::string& phone,const std::string& email,const std::string& postscript,const std::string& platform,const std::string& provider,const std::string& url,const std::string& hotline, const std::string& category);

    /**
     * @brief 删除目标账号密码记录
     * @param msg 反馈信息
     * @param accountId 目标账号
     * @return 成功与否
     */
    bool deleteAccount(std::string& msg, int accountId);

private:
    /**
     * @brief mainPwdToRealKey
     * @param mainPwd 主密码
     * @return 计算结果
     */
    const std::vector<unsigned char> mainPwdToRealKey(const std::string& mainPwd) const;
};



#endif
