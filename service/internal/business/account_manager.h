#ifndef ACCOUNT_MANAGER_H
#define ACCOUNT_MANAGER_H

#include <string>
#include <vector>
#include "view_account.h"

namespace zinpass::business{
class AccountManager {
public:
    template<typename T>
    struct Return {
        T data;
        std::string message;
    };

    /**
     * @brief getEmailOptions
     * @param sys_user_id 限制范围，目标用户
     * @return email_addresses 获取到的邮箱地址 + message
     */
    static Return<std::vector<std::string>> get_email_options(const std::string& sys_user_id);

    /**
     * @brief getCategoryOptions
     * @return categories + message
     */
    static Return<std::vector<std::string>> get_category_options();

    /**
     * @brief getAccounts 获取所有view_accounts信息
     * @param provider_name 服务商名称
     * @param platform_name 平台名称
     * @param username 账号用户名
     * @param nickname 别称
     * @param telephone 手机号
     * @param email 邮箱
     * @param category 类别
     * @param postscript 备注
     * @param sys_user_id 系统用户
     * @return view_accounts + message
     */
    static Return<std::vector<models::ViewAccount>> get_accounts(
        const std::string& provider_name,
        const std::string& platform_name,
        const std::string& username,
        const std::string& nickname,
        const std::string& telephone,
        const std::string& email,
        const std::string& category,
        const std::string& postscript,
        const std::string& sys_user_id
        );

    /**
     * @brief 根据id主键获取view_accounts信息
     * @param account_id 目标
     * @return view_account + message
     */
    static Return<models::ViewAccount> get_account_by_id(const std::string& account_id);

    /**
     * @brief 获取账号密码的密文并解密，返回明文
     * @param account_id 反馈信息
     * @param sys_user_id 账号所属人
     * @param main_password 主密码
     * @return plaintext + message
     */
    static Return<std::string> retrieve_decrypted_password(const std::string& account_id, const std::string& sys_user_id, const std::string& main_password);

    /**
     * @brief 新增账号记录
     * @param main_password 主密码
     * @param username 账号用户名
     * @param nickname 昵称
     * @param password 密码
     * @param sub_account 子账号
     * @param phone 绑定手机号
     * @param email 绑定邮箱
     * @param postscript 备注
     * @param platform 平台名
     * @param provider 服务商
     * @param url 网址
     * @param hotline 客服热线
     * @param sys_user_id 账号所属用户
     * @param category 类别
     * @return 成功与否
     */
    [[deprecated("This function is obsolete. Use addViewAccount() instead.")]]
    static Return<bool> add_account(
        const std::string& main_password,
        const std::string& username,
        const std::string& nickname,
        const std::string& password,
        const std::string& sub_account,
        const std::string& phone,
        const std::string& email,
        const std::string& postscript,
        const std::string& platform, const std::string& provider, const std::string& url,
        const std::string& hotline, const std::string& sys_user_id, const std::string& category);

    /**
     * @brief 新增账号记录（注意，会修改view_account对象的部分值）
     * @param main_password 主密码
     * @param view_account 新增对象（主键自动生成，无需赋值）
     * @param plaintext_pwd 明文密码
     * @return 成功与否 + message
     */
    static Return<bool> add_view_account(
        const std::string& main_password,
        models::ViewAccount& view_account,
        const std::string& plaintext_pwd
    );

    /**
     * @brief 更新账号信息（除创建时间、记录所属用户）
     * @param view_account 待更新的账号信息（根据属性 id 和 sys_user_id 修改， 不需要 encrypted_pwd 和 iv）
     * @param plaintext_pwd 明文密码（新）
     * @param main_password 主密码
     * @return 成功与否 + message
     */
    static Return<bool> update_account(const models::ViewAccount& view_account,const std::string& plaintext_pwd,const std::string& main_password);

    /**
     * @brief 更新账号的基本信息（除创建时间、记录所属用户、密码密文、初始化向量值）
     * @param view_account 待更新的账号信息（根据属性 id 和 sys_user_id 修改， 不需要 encrypted_pwd 和 iv）
     * @return 成功与否 + message
     */
    static Return<bool> update_account_base_info(const models::ViewAccount& view_account);

    /**
     * @brief 更新账号的密码（仅更新密码密文、初始化向量值）
     * @param account_id 更新目标
     * @param sys_user_id 目标所属用户（若不正确不会更新）
     * @param main_password 所属用户的主密码
     * @param plaintext_pwd 明文密码（新）
     * @return 成功与否 + message
     */
    static Return<bool> update_account_password(const std::string& account_id,const std::string& sys_user_id,const std::string& main_password,const std::string& plaintext_pwd);

    /**
     * @brief 删除目标账号密码记录
     * @param account_id 目标账号
     * @return 成功与否 + message
     */
    static Return<bool> delete_account(const std::string& account_id);

private:
    /**
     * @brief mainPasswordToRealKey
     * @param main_password 主密码
     * @return 计算结果
     */
    static std::vector<unsigned char> main_password_to_real_key(const std::string& main_password);
};

}

#endif
