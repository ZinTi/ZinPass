#include "account_manager.h"
#include "account_dao.h"
#include "category_dao.h"
#include "date_time.h"
#include "mobile_phone_dao.h"
#include "pwd_utils.h"

namespace zinpass::business{

std::vector<unsigned char> AccountManager::main_password_to_real_key(const std::string& main_password){
    const std::string realKeyHex = utils::PwdUtils::calculateMD5(main_password);
    const std::vector<unsigned char> realKey = utils::PwdUtils::hexStringToKey(realKeyHex);
    return realKey;
}

AccountManager::Return<std::vector<std::string>> AccountManager::get_email_options(const std::string& sys_user_id) {
    std::vector<std::string> email_addresses;
    std::string message;
    try {
        repository::AccountDAO email_dao;
        if (const repository::DaoStatus dao_ret = email_dao.findEmailList(sys_user_id, email_addresses);
            dao_ret == repository::DaoStatus::Success)
        {
            message = "成功";
        } else if (dao_ret == repository::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {email_addresses, message};
}

AccountManager::Return<std::vector<std::string>> AccountManager::get_category_options() {
    std::vector<std::string> categories;
    std::string message;
    try {
        repository::CategoryDAO category_dao;
        if (const repository::DaoStatus dao_ret = category_dao.findCategories(categories);
            dao_ret == repository::DaoStatus::Success)
        {
            message = "成功";
        } else if (dao_ret == repository::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {categories, message};
}

AccountManager::Return<std::vector<models::ViewAccount>> AccountManager::get_accounts(
    const std::string& provider_name,
    const std::string& platform_name,
    const std::string& username,
    const std::string& nickname,
    const std::string& telephone,
    const std::string& email,
    const std::string& category,
    const std::string& postscript,
    const std::string& sys_user_id
) {
    std::vector<models::ViewAccount> view_accounts;
    std::string message;
    try {
        repository::AccountDAO account_dao;
        if (const repository::DaoStatus dao_ret = account_dao.find(provider_name,platform_name,
            username,nickname,telephone,email,category,postscript,sys_user_id,view_accounts);
            dao_ret == repository::DaoStatus::Success) {
            message = "成功";
        } else if (dao_ret == repository::DaoStatus::NotFound) {
            message = "未查询到";
        } else {
            message = "发生错误";
        }
    }catch(const std::exception& e) {
        message = e.what();
    }
    return {view_accounts, message};
}

AccountManager::Return<models::ViewAccount> AccountManager::get_account_by_id(const std::string& account_id) {
    models::ViewAccount view_account;
    std::string message;
    const repository::AccountDAO account_dao;
    if (const repository::DaoStatus ret = account_dao.findById(account_id, view_account);
        repository::DaoStatus::Success != ret
        ) {
        message = "获取失败: " + std::to_string(static_cast<int>(ret));
    }else {
        message = "成功";
    }
    return {view_account, message};
}

AccountManager::Return<std::string> AccountManager::retrieve_decrypted_password(const std::string& account_id, const std::string& sys_user_id, const std::string& main_password) {
    // 1. 从数据库读取数据
    std::vector<unsigned char> ciphertext, iv;
    const repository::AccountDAO account_dao;
    account_dao.findEncryptedPwdAndIv(account_id, sys_user_id, ciphertext, iv);
    if (ciphertext.empty()) {
        return {"查询目标不存在", ""};
    }

    // 2. 解密
    Return<std::string> ret;
    if(std::string plaintext;
        utils::PwdUtils::decryptTextByAES128CTR(ciphertext, iv, main_password_to_real_key(main_password), plaintext))
    {
        ret.data = plaintext;
        ret.message = "成功";
    }else{
        ret.data = "";
        ret.message = "解密失败";
    }
    return ret;
}

[[deprecated("This function is obsolete. Use add_view_account() instead.")]]
AccountManager::Return<bool> AccountManager::add_account(
    const std::string& main_password,
    const std::string& username,
    const std::string& nickname,
    const std::string& password,
    const std::string& sub_account,
    const std::string& phone,
    const std::string& email,
    const std::string& postscript,
    const std::string& platform,
    const std::string& provider,
    const std::string& url,
    const std::string& hotline,
    const std::string& sys_user_id,
    const std::string& category)
{
    // 0\ 定义部分变量

    // 1\ 校验参数数据合法性

    // 2\ 加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        password,                               // 原明文密码
        main_password_to_real_key(main_password),   // 由主密码生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (false == ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    // 3\ 将 phone -> phone_id , 将 email -> email_id , 将 category -> category_id， 获取时间
    int phone_id = -1;
    std::string email_id = "";
    short category_id = -1;

    if (phone != "无" && phone != "空" && phone != "NULL") {
        const repository::MobilePhoneDAO phone_dao;
        if (const int temp_phone_id = phone_dao.find_id_by_number(phone);
            temp_phone_id >= 0) {
            phone_id = temp_phone_id;
        }
    }

    if (email != "无" && email != "空" && email != "NULL") {
        const repository::AccountDAO email_dao;
        email_id = email_dao.getIdByEmailAddress(email);
    }

    const repository::CategoryDAO category_dao;
    short temp_category_id;
    if (const repository::DaoStatus ret_category_dao = category_dao.categoryNameToId(category, &temp_category_id);
        repository::DaoStatus::Success == ret_category_dao) {
        category_id = temp_category_id;
    }

    const std::string current_date_time = utils::DateTime::getCurrentDateTime();

    // 4\ 创建对象并写入到数据库中
    const models::Account account(
        0,                  // 底层函数会自动分配主键
        username,           // 参数
        nickname,           // 参数
        sub_account,        // 参数
        postscript,         // 参数
        platform,           // 参数
        provider,           // 参数
        url,                // 参数
        hotline,            // 参数
        sys_user_id,           // 参数
        current_date_time,  // 当前时间
        current_date_time,  // 当前时间
        ciphertext,         // 已生成
        initialization_vector,// 已生成
        phone_id,
        email_id,
        category_id
        );

    const repository::AccountDAO account_dao;
    if (repository::DaoStatus ret_account_dao = account_dao.add(account);
        repository::DaoStatus::Success != ret_account_dao
        ) {
        return {false, "失败" + std::to_string(static_cast<int>(ret_account_dao))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::add_view_account(
    const std::string& main_password,
    models::ViewAccount& view_account,
    const std::string& plaintext_pwd)
{
    // 0\ 定义部分变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();

    // 1\ 校验参数数据合法性
    if (plaintext_pwd.empty()) {
        return {false, "密码不得为空"};
    }

    // 2\ 加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_password_to_real_key(main_password),   // 由主密码生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (false == ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    // 3\ 更新对象部分属性并写入到数据库中
    view_account.setEncryptedPwd(ciphertext);
    view_account.setIv(initialization_vector);
    view_account.setCreatedTime(current_date_time);
    view_account.setUpdatedTime(current_date_time);

    const repository::AccountDAO account_dao;
    if (repository::DaoStatus ret_account_dao = account_dao.add(view_account);
        repository::DaoStatus::Success != ret_account_dao
        ) {
        return {false, "失败" + std::to_string(static_cast<int>(ret_account_dao))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account(
    const models::ViewAccount& view_account,
    const std::string& plaintext_pwd,
    const std::string& main_password)
{
    // 0\ 定义部分变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    repository::AccountDAO account_dao;
    models::ViewAccount new_view_account(view_account);

    // 1\ 检查输入数据的合法性
    if (view_account.getId().empty()) {
        return {false, "因不合法主键中断更新账号数据，主键长度为0"};
    }

    // 2\ 新密码：加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_password_to_real_key(main_password),   // 由主密码生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (false == ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }
    new_view_account.setEncryptedPwd(ciphertext);
    new_view_account.setIv(initialization_vector);
    new_view_account.setUpdatedTime(current_date_time);

    if (const repository::DaoStatus ret_account_dao_2 = account_dao.update(new_view_account);
        repository::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account_base_info(const models::ViewAccount& view_account){
    // 0\ 定义部分变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    const repository::AccountDAO account_dao;

    // 1\ 检查输入数据的合法性
    if (view_account.getId().empty()) {
        return {false, "因不合法主键中断更新账号数据，主键长度为0"};
    }

    if (const repository::DaoStatus ret_account_dao_2 = account_dao.update_main_properties(view_account);   // 忽略 encrypted_pwd 和 iv
        repository::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

AccountManager::Return<bool> AccountManager::update_account_password(const std::string& account_id,
    const std::string& sys_user_id,const std::string& main_password,const std::string& plaintext_pwd)
{
    // 0\ 定义部分变量
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    const repository::AccountDAO account_dao;

    // 1\ 检查输入数据的合法性
    if (main_password.empty()) {
        return {false, "主密码不能为空"};
    }
    if (plaintext_pwd.empty()) {
        return {false, "账号新密码不能为空"};
    }

    // 2\ 新密码：加密 "明文密码" 和 生成 "iv"
    std::vector<unsigned char> ciphertext, initialization_vector;
    const bool ret_encrypt = utils::PwdUtils::encryptTextByAES128CTR(
        plaintext_pwd,                          // 原明文密码
        main_password_to_real_key(main_password),   // 由主密码生成 key
        ciphertext,                         // 生成的密文密码（待存储）
        initialization_vector               // 生成的初始化向量（待存储）
        );
    if (false == ret_encrypt) {
        return {false, "因生成密文密码失败，中断存储账号记录的操作"};
    }

    if (const repository::DaoStatus ret_account_dao_2 = account_dao.update_password(account_id, sys_user_id, ciphertext, initialization_vector, current_date_time);
        repository::DaoStatus::Success != ret_account_dao_2) {
        return {false, "更新失败：" + std::to_string(static_cast<int>(ret_account_dao_2))};
    }
    return {true, "成功"};
}

/*
AccountManager::Return<bool> AccountManager::update_account(
    const std::string& account_id,
    const std::string& username,
    const std::string& nickname,
    const std::string& pwd,
    const std::string& subAccount,
    const std::string& phone,
    const std::string& email,
    const std::string& postscript,
    const std::string& platform,
    const std::string& provider,
    const std::string& url,
    const std::string& hotline)
{
    bool ret;
    std::vector<AccountDAO::KeyValuePair> data;
    // 设置需要更新的数据
    //updateData[AccountDAO::ColumnType::id] = account_id;
    data.push_back({AccountDAO::ColumnType::username, username});
    data.push_back({AccountDAO::ColumnType::nickname, nickname});
    if (false == pwd.empty()) {
        data.push_back({AccountDAO::ColumnType::password, pwd});
    }
    data.push_back({AccountDAO::ColumnType::subAccount, subAccount});
    short phoneId;
    std::string emailId;
    std::string phoneMsg, emailMsg;
    MobilePhoneDAO phoneDAO;
    AccountDAO emailDAO;
    if (phoneDAO.phoneNumberToId(phoneMsg, phone, &phoneId)) {
        data.push_back({AccountDAO::ColumnType::phoneId, phoneId});
    }
    if (emailId = emailDAO.getIdByEmailAddress(email) && !emailId.empty()) {
        data.push_back({AccountDAO::ColumnType::emailId, emailId});
    }
    data.push_back({AccountDAO::ColumnType::postscript, postscript});
    data.push_back({AccountDAO::ColumnType::platformName, platform});
    data.push_back({AccountDAO::ColumnType::providerName, provider});
    data.push_back({AccountDAO::ColumnType::URL, url});
    data.push_back({AccountDAO::ColumnType::hotline, hotline});
    data.push_back({AccountDAO::ColumnType::owner, phoneId});
    data.push_back({AccountDAO::ColumnType::updatedTime, getCurrentDateTime()});
    //
    AccountDAO account_dao;
    if (account_dao.update(message, data, account_id)) {
        message =  "更新成功：" + message;
        ret = true;
    } else {
        message = "更新失败：" + message;
        ret = false;
    }
    return ret;
}*/

AccountManager::Return<bool> AccountManager::delete_account(const std::string& account_id) {
    const repository::AccountDAO account_dao;
    if (const repository::DaoStatus ret_account_dao = account_dao.remove(account_id);
        repository::DaoStatus::Success != ret_account_dao)
    {
        return {false, "删除失败：" + std::to_string(static_cast<int>(ret_account_dao))};
    }
    return {true, "删除成功"};
}

} // zinpass::business