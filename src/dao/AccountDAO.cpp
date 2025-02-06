#include <string>
#include <sstream>
#include <map>
#include "AccountDAO.h"
#include "sqlite3.h"
#include <iostream>

AccountDAO::AccountDAO(const std::string& dbPath) : dbPath(dbPath) {
    this->db = nullptr;
}

AccountDAO::~AccountDAO() = default;

void AccountDAO::openDatabase(std::string& msg) {
    if (sqlite3_open(dbPath.c_str(), &this->db) != SQLITE_OK) {
        msg = "Failed to open database: " + std::string(sqlite3_errmsg(this->db));
    }
    else {
        msg = "Database opened successfully.";
    }
}

void AccountDAO::closeDatabase() const {
    sqlite3_close(this->db);
}

void AccountDAO::stmtToViewAccount(sqlite3_stmt* stmt, ViewAccount& viewAccount) {
    if (sqlite3_column_type(stmt, 0) == SQLITE_NULL) {
        viewAccount.setId(0);
    }
    else {
        viewAccount.setId(sqlite3_column_int(stmt, 0));
    }
    const auto tProviderName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    viewAccount.setProviderName(tProviderName == nullptr ? "" : tProviderName);
    const auto tPlatformName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    viewAccount.setPlatformName(tPlatformName == nullptr ? "" : tPlatformName);
    const auto tUserId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
    viewAccount.setUserId(tUserId == nullptr ? "" : tUserId);
    const auto tAlias = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
    viewAccount.setAlias(tAlias == nullptr ? "" : tAlias);
    const auto tSubAccount = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
    viewAccount.setSubAccount(tSubAccount == nullptr ? "" : tSubAccount);
    const auto tPhone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
    viewAccount.setPhone(tPhone == nullptr ? "" : tPhone);
    const auto tEmail = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
    viewAccount.setEmail(tEmail == nullptr ? "" : tEmail);
    const auto tPostscript = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8));
    viewAccount.setPostscript(tPostscript == nullptr ? "" : tPostscript);
    const auto tUrl = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9));
    viewAccount.setURL(tUrl == nullptr ? "" : tUrl);
    const auto tHotline = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10));
    viewAccount.setHotline(tHotline == nullptr ? "" : tHotline);
    const auto tCategory = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 11));
    viewAccount.setCategory(tCategory == nullptr ? "" : tCategory);
    const auto tCreatedTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
    viewAccount.setCreatedTime(tCreatedTime == nullptr ? "" : tCreatedTime);
    const auto tUpdatedTime = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 13));
    viewAccount.setUpdatedTime(tUpdatedTime == nullptr ? "" : tUpdatedTime);
    const auto tSysUserId = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 14));
    viewAccount.setSysUserId(tSysUserId == nullptr ? "" : tSysUserId);
}

std::string AccountDAO::buildSqlPart(const std::map<ColumnType, std::string>& data, const std::string& separator) {
    std::string part;
    for (const auto& [col, value] : data) {
        std::stringstream ss;
        switch (col) {
        case ColumnType::id:
            ss << "id =?";
            break;
        case ColumnType::userId:
            ss << "user_id =?";
            break;
        case ColumnType::alias:
            ss << "alias =?";
            break;
        case ColumnType::password:
            ss << "pwd =?";
            break;
        case ColumnType::subAccount:
            ss << "sub_account =?";
            break;
        case ColumnType::phoneId:
            ss << "phone_id =?";
            break;
        case ColumnType::emailId:
            ss << "email_id =?";
            break;
        case ColumnType::postscript:
            ss << "postscript =?";
            break;
        case ColumnType::platformName:
            ss << "platform_name =?";
            break;
        case ColumnType::providerName:
            ss << "provider_name =?";
            break;
        case ColumnType::URL:
            ss << "URL =?";
            break;
        case ColumnType::hotline:
            ss << "hotline =?";
            break;
        case ColumnType::categoryId:
            ss << "category_id =?";
            break;
        case ColumnType::owner:
            ss << "sys_user_id =?";
            break;
        case ColumnType::createdTime:
            ss << "created_time =?";
            break;
        case ColumnType::updatedTime:
            ss << "updated_time =?";
            break;
        default:
            continue;
        }
        if (!part.empty()) {
            part += separator;
        }
        part += ss.str();
    }
    return part;
}

int AccountDAO::generateId(std::string& msg) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    int id = 0;

    // 2. 首先获取目前表中最大的 id
    const std::string sql_find_unused =
        "SELECT MIN(id) AS first_unused_id FROM (SELECT t1.id + 1 AS id FROM account t1 LEFT JOIN account t2 ON t1.id + 1 = t2.id WHERE t2.id IS NULL AND t1.id < (SELECT MAX(id) FROM account) UNION SELECT (SELECT MAX(id) FROM account) + 1 AS id) AS subquery;";
    const int rc = sqlite3_prepare_v2(db, sql_find_unused.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "Prepare failed: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return -1;
    }
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    closeDatabase();
    return id;
}

bool AccountDAO::findById(std::string& msg, int id, ViewAccount& viewAccount) {
    bool result;
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const char* sql = "SELECT * FROM view_account WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    const int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "Prepare failed: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }
    sqlite3_bind_int(stmt, 1, id); // 绑定参数

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stmtToViewAccount(stmt, viewAccount);
        msg = "Record found.";
        result = true;
    }
    else {
        msg = "No record found with the given ID.";
        result = false;
    }

    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

[[deprecated("不推荐使用，使用另一个函数代替")]]
bool AccountDAO::find(std::string& msg, const std::string& sysUserId, std::vector<ViewAccount>& viewAccounts) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const std::string sql = "SELECT * FROM view_account WHERE sys_user_id = \'" + sysUserId + "\'";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ViewAccount viewAccount;
        stmtToViewAccount(stmt, viewAccount);
        viewAccounts.push_back(viewAccount);
    }

    msg = "All records fetched.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

// 标记此函数不进行优化。这里开启优化后总是出现只查询到最多1条记录的bug，初步定位到问题所在：优化sql字符串 或 优化绑定sql参数 时出问题。
__attribute__((optimize("O0")))
bool AccountDAO::find(std::string& msg, const std::string& providerName, const std::string& platformName,
                      const std::string& userId, const std::string& alias, const std::string& phone,
                      const std::string& email, const std::string& updateTimeRangeStart, const std::string& updateTimeRangeEnd,
                      const std::string& sysUserId, std::vector<ViewAccount>& viewAccounts
) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }
    const std::string sql =  R"(SELECT * FROM view_account
        WHERE (provider_name LIKE ? OR (? = '' AND provider_name IS NULL))
        AND (platform_name LIKE ? OR (? = '' AND platform_name IS NULL))
        AND (user_id LIKE ? OR (? = '' AND user_id IS NULL))
        AND (alias LIKE ? OR (? = '' AND alias IS NULL))
        AND (phone = ? OR (? = '' AND (phone IS NOT NULL OR phone IS NULL)))
        AND (email = ? OR (? = '' AND (email IS NOT NULL OR email IS NULL)))
        AND ((updated_time > ? AND updated_time < ?) OR (? = '' AND ? = ''))
        AND (sys_user_id = ? OR (? = '' AND sys_user_id IS NULL));)";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    // Bind parameters and their corresponding "is NULL" conditions
    sqlite3_bind_text(stmt, 1, ("%" + providerName + "%").c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, providerName.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 3, ("%" + platformName + "%").c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, platformName.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 5, ("%" + userId + "%").c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, userId.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 7, ("%" + alias + "%").c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 8, alias.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 9, phone.c_str(), -1, SQLITE_STATIC); // phone是精确而非模糊筛选（当为空时则不筛选）
    sqlite3_bind_text(stmt, 10, phone.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 11, email.c_str(), -1, SQLITE_STATIC); // email是精确而非模糊筛选（当为空时则不筛选）
    sqlite3_bind_text(stmt, 12, email.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 13, updateTimeRangeStart.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 14, updateTimeRangeEnd.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 15, updateTimeRangeStart.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 16, updateTimeRangeEnd.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 17, sysUserId.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 18, sysUserId.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ViewAccount viewAccount;
        stmtToViewAccount(stmt, viewAccount);
        viewAccounts.push_back(viewAccount);
    }

    msg = "Records fetched based on the search criteria.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

/**
     * @brief 根据id查询目标account的密码
     * @param msg 反馈信息
     * @param id 查询目标的id
     * @param encryptedPwd 获取到的密码
     * @param iv 查询到的初始化向量值
     * @param sysUserId 获取到的密码记录的所属用户
     * @return 成功与否
     */
bool AccountDAO::findEncryptedPwdAndIvById(std::string& msg, int id, std::vector<unsigned char>& encryptedPwd, std::vector<unsigned char>& iv, std::string& sysUserId){
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    sqlite3_stmt* stmt = nullptr;
    // 2. 首先获取目前表中最大的 id
    const std::string sql = "SELECT encrypted_pwd, iv, sys_user_id FROM view_pwd WHERE id =?";
    const int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "Prepare failed: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }
    // 绑定参数
    sqlite3_bind_int(stmt, 1, id);
    // 执行 SQL 语句
    const int stepResult = sqlite3_step(stmt);
    if (stepResult == SQLITE_ROW) {
        const int blobSizeEncryptedPwd = sqlite3_column_bytes(stmt, 0);
        const auto* blobPtrEncryptedPwd = static_cast<const unsigned char*>(sqlite3_column_blob(stmt, 0));
        encryptedPwd.assign(blobPtrEncryptedPwd, blobPtrEncryptedPwd + blobSizeEncryptedPwd);

        const int blobSizeIV = sqlite3_column_bytes(stmt, 1);
        const auto* blobPtrIV = static_cast<const unsigned char*>(sqlite3_column_blob(stmt, 1));
        iv.assign(blobPtrIV, blobPtrIV + blobSizeIV);

        // 安全地获取 sys_user_id 列数据
        const char* sysUser = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        sysUserId = sysUser ? std::string(sysUser) : "";
    }
    else if (stepResult == SQLITE_DONE) {
        msg = "No rows found";
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }
    else {
        msg = "Step failed: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }
    sqlite3_finalize(stmt);

    closeDatabase();
    msg = "查询完成";
    return true;
}

// 标记此函数不进行优化。
__attribute__((optimize("O0")))
bool AccountDAO::add(std::string& msg, const Account& account) {
    bool result = false;
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    char const* sql = "INSERT INTO account (id, user_id, alias, encrypted_pwd, iv, sub_account, phone_id, email_id, postscript, platform_name, provider_name, URL, hotline, category_id, sys_user_id, created_time, updated_time) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    std::string tMsg;
    sqlite3_bind_int(stmt, 1, generateId(tMsg)); // 生成id主键, NOT NULL
    if (account.getUserId().empty()) {
        sqlite3_bind_null(stmt, 2);
    }
    else {
        sqlite3_bind_text(stmt, 2, account.getUserId().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getAlias().empty()) {
        sqlite3_bind_null(stmt, 3);
    }
    else {
        sqlite3_bind_text(stmt, 3, account.getAlias().c_str(), -1, SQLITE_STATIC);
    }

    // 绑定BLOB数据到预处理语句
    sqlite3_bind_blob(stmt, 4, account.getEncryptedPwd().data(), static_cast<int>(account.getEncryptedPwd().size()), SQLITE_STATIC);
    sqlite3_bind_blob(stmt, 5, account.getIv().data(), static_cast<int>(account.getIv().size()), SQLITE_STATIC);

    if (account.getSubAccount().empty()) {
        sqlite3_bind_null(stmt, 6);
    }
    else {
        sqlite3_bind_text(stmt, 6, account.getSubAccount().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getPhoneId() < 0) {
        sqlite3_bind_null(stmt, 7);
    }
    else {
        sqlite3_bind_int(stmt, 7, account.getPhoneId());
    }
    if (account.getEmailId() < 0) {
        sqlite3_bind_null(stmt, 8);
    }
    else {
        sqlite3_bind_int(stmt, 8, account.getEmailId());
    }
    if (account.getPostscript().empty()) {
        sqlite3_bind_null(stmt, 9);
    }
    else {
        sqlite3_bind_text(stmt, 9, account.getPostscript().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getPlatformName().empty()) {
        sqlite3_bind_null(stmt, 10);
    }
    else {
        sqlite3_bind_text(stmt, 10, account.getPlatformName().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getProviderName().empty()) {
        sqlite3_bind_null(stmt, 11);
    }
    else {
        sqlite3_bind_text(stmt, 11, account.getProviderName().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getURL().empty()) {
        sqlite3_bind_null(stmt, 12);
    }
    else {
        sqlite3_bind_text(stmt, 12, account.getURL().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getHotline().empty()) {
        sqlite3_bind_null(stmt, 13);
    }
    else {
        sqlite3_bind_text(stmt, 13, account.getHotline().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getCategoryId() < 0) {
        sqlite3_bind_null(stmt, 14);
    }
    else {
        sqlite3_bind_int(stmt, 14, account.getCategoryId());
    }
    if (account.getSysUserId().empty()) {
        sqlite3_bind_null(stmt, 15);
    }
    else {
        sqlite3_bind_text(stmt, 15, account.getSysUserId().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getCreatedTime().empty()) {
        sqlite3_bind_null(stmt, 16);
    }
    else {
        sqlite3_bind_text(stmt, 16, account.getCreatedTime().c_str(), -1, SQLITE_STATIC);
    }
    if (account.getUpdatedTime().empty()) {
        sqlite3_bind_null(stmt, 17);
    }
    else {
        sqlite3_bind_text(stmt, 17, account.getUpdatedTime().c_str(), -1, SQLITE_STATIC);
    }

    if (
        sqlite3_step(stmt) != SQLITE_DONE
        ) {
        msg = "Failed to insert record: " + std::string(sqlite3_errmsg(db));
        result = false;
    }else {
        msg = "Record inserted successfully.";
        result = true;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

/**
 * @brief 修改Account表中的产品信息（1/4）
 */
// 标记此函数不进行优化。
__attribute__((optimize("O0")))
bool AccountDAO::update(std::string& msg, const int id, const std::string& providerName,
                        const std::string& platformName, const std::string& url, const std::string& hotline, const std::string& updateTime) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const char* sql =
        "UPDATE account SET provider_name = ?, platform_name = ?, URL = ?, hotline = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    if (providerName.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, providerName.c_str(), -1, SQLITE_STATIC);
    if (platformName.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, platformName.c_str(), -1, SQLITE_STATIC);
    if (url.empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, url.c_str(), -1, SQLITE_STATIC);
    if (hotline.empty()) sqlite3_bind_null(stmt, 4);
    else sqlite3_bind_text(stmt, 4, hotline.c_str(), -1, SQLITE_STATIC);
    if (updateTime.empty()) {
        msg = "更新时间不能为空";
        return false;
    }
    else { sqlite3_bind_text(stmt, 5, updateTime.c_str(), -1, SQLITE_STATIC); }
    sqlite3_bind_int(stmt, 6, id); // NOT NULL

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to update record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record updated successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

/**
 * @brief 修改Account表中的账号信息（2/4）
 */
// 标记此函数不进行优化。
__attribute__((optimize("O0")))
bool AccountDAO::update(std::string& msg, int id, const std::string& userId, const std::string& alias, const std::vector<unsigned char>& encryptedPwd, const std::vector<unsigned char>& iv, short categoryId, const std::string& updateTime){
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const char* sql =
        "UPDATE account SET user_id = ?, alias = ?, encrypted_pwd = ?, iv = ?, category_id = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    if (userId.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);
    if (alias.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, alias.c_str(), -1, SQLITE_STATIC);
    if (encryptedPwd.empty()) {
        msg = "密码不能为空";
        return false;
    }else{
        sqlite3_bind_blob(stmt, 3, encryptedPwd.data(), static_cast<int>(encryptedPwd.size()), SQLITE_STATIC);
    }
    if (iv.empty()) {
        msg = "初始化向量不能为空";
        return false;
    }else{
        sqlite3_bind_blob(stmt, 4, iv.data(), static_cast<int>(iv.size()), SQLITE_STATIC);
    }
    if (categoryId < 0) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_int(stmt, 5, categoryId);
    if (updateTime.empty()) {
        msg = "更新时间不能为空";
        return false;
    }
    else { sqlite3_bind_text(stmt, 6, updateTime.c_str(), -1, SQLITE_STATIC); }
    sqlite3_bind_int(stmt, 7, id); // NOT NULL

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to update record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record updated successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

/**
 * @brief 修改Account表中的第三方绑定账号信息（3/4）
 */
// 标记此函数不进行优化。
__attribute__((optimize("O0")))
bool AccountDAO::update(std::string& msg, const int id, const int phoneId, const int emailId,
                        const std::string& updateTime) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const char* sql = "UPDATE account SET phone_id = ?, email_id = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    if (phoneId < 0) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_int(stmt, 1, phoneId);
    if (emailId < 0) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_int(stmt, 2, emailId);
    if (updateTime.empty()) {
        msg = "更新时间不能为空";
        return false;
    }
    else { sqlite3_bind_text(stmt, 3, updateTime.c_str(), -1, SQLITE_STATIC); }
    sqlite3_bind_int(stmt, 4, id); // NOT NULL

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to update record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record updated successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

/**
 * @brief 修改Account表中的其他信息（4/4）
 */
// 标记此函数不进行优化。
__attribute__((optimize("O0")))
bool AccountDAO::update(std::string& msg, const int id, const std::string& subAccount, const std::string& postscript,
                        const std::string& updateTime) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const char* sql = "UPDATE account SET sub_account = ?, postscript = ?, updated_time = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    if (subAccount.empty()) sqlite3_bind_null(stmt, 1);
    else sqlite3_bind_text(stmt, 1, subAccount.c_str(), -1, SQLITE_STATIC);
    if (postscript.empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, postscript.c_str(), -1, SQLITE_STATIC);
    if (updateTime.empty()) {
        msg = "更新时间不能为空";
        return false;
    }
    else { sqlite3_bind_text(stmt, 3, updateTime.c_str(), -1, SQLITE_STATIC); }
    sqlite3_bind_int(stmt, 4, id); // NOT NULL

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to update record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record updated successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

// 新版本 [有很多Bug]
bool AccountDAO::update(std::string& msg, const std::vector<KeyValuePair>& data, const int id) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    std::stringstream sqlStream;
    sqlStream << "UPDATE account SET ";
    bool first = true;
    std::vector<int> textIndices;
    std::vector<int> intIndices;
    for (const auto& kv : data) {
        if (!first) {
            sqlStream << ", ";
        }
        switch (kv.key) {
        case ColumnType::userId: {
            sqlStream << "user_id =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::alias: {
            sqlStream << "alias =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::password: {
            sqlStream << "pwd =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::subAccount: {
            sqlStream << "sub_account =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::phoneId: {
            sqlStream << "phone_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::emailId: {
            sqlStream << "email_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::postscript: {
            sqlStream << "postscript =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::platformName: {
            sqlStream << "platform_name =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::providerName: {
            sqlStream << "provider_name =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::URL: {
            sqlStream << "URL =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::hotline: {
            sqlStream << "hotline =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::categoryId: {
            sqlStream << "category_id =?";
            intIndices.push_back(static_cast<int>(intIndices.size()) + 1);
            break;
        }
        case ColumnType::owner: {
            sqlStream << "sys_user_id =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        case ColumnType::updatedTime: {
            sqlStream << "updated_time =?";
            textIndices.push_back(static_cast<int>(textIndices.size()) + 1);
            break;
        }
        default: {
            // 可以考虑更合适的处理方式，这里暂时不做任何操作
            break;
        }
        }
        first = false;
    }
    sqlStream << " WHERE id =?";

    const std::string sql = sqlStream.str();
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    for (const int index : textIndices) {
        for (const auto& [key, value] : data) {
            switch (key) {
            case ColumnType::userId: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                // 可以添加错误处理，例如类型不匹配的情况
                break;
            }
            case ColumnType::alias: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::password: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::subAccount: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::postscript: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::platformName: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::providerName: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::URL: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::hotline: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::owner: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            case ColumnType::updatedTime: {
                if (std::holds_alternative<std::string>(value)) {
                    sqlite3_bind_text(stmt, index, std::get<std::string>(value).c_str(), -1, SQLITE_STATIC);
                    break;
                }
                break;
            }
            default: {
                // 可以考虑更合适的处理方式，这里暂时不做任何操作
                break;
            }
            }
        }
    }

    for (const int index : intIndices) {
        for (const auto& kv : data) {
            switch (kv.key) {
            case ColumnType::phoneId: {
                if (std::holds_alternative<int>(kv.value)) {
                    sqlite3_bind_int(stmt, index, std::get<int>(kv.value));
                    break;
                }
                // 可以添加错误处理，例如类型不匹配的情况
                break;
            }
            case ColumnType::emailId: {
                if (std::holds_alternative<int>(kv.value)) {
                    sqlite3_bind_int(stmt, index, std::get<int>(kv.value));
                    break;
                }
                break;
            }
            default: {
                // 可以考虑更合适的处理方式，这里暂时不做任何操作
                break;
            }
            }
        }
    }

    sqlite3_bind_int(stmt, static_cast<int>(textIndices.size() + intIndices.size() + 1), id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to update record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record updated successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool AccountDAO::remove(std::string& msg, const int id) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return false;
    }

    const std::string sql = "DELETE FROM account WHERE id = ?";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "Failed to prepare statement: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "Failed to delete record: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "Record deleted successfully.";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

// Email 部分（特殊的 Account）
bool AccountDAO::findEmailList(std::string& msg, const std::string& sysUserId,
                               std::vector<std::string>& emailAddrList) {
    openDatabase(msg);
    if (!db) return false;

    const std::string sql = "SELECT user_id FROM view_email WHERE sys_user_id = \'" + sysUserId + "\'";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        emailAddrList.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    msg = "查询到电子邮箱数：" + std::to_string(emailAddrList.size());
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool AccountDAO::emailAddressToId(std::string& msg, const std::string& emailAddr, short* id) {
    bool result;
    openDatabase(msg);
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, "SELECT id FROM view_email WHERE user_id = ?", -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, emailAddr.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *id = static_cast<short>(sqlite3_column_int(stmt, 0));
        msg = "查询成功";
        result = true;
    }
    else {
        *id = -1;
        msg = "未找到指定邮箱的id, 赋值 -1";
        result = false;
    }

    sqlite3_finalize(stmt); // 这里为什么会发生段错误？
    closeDatabase();
    return result;
}

// 根据 id 查询目标 email 记录
bool AccountDAO::findEmailById(std::string& msg, const int id, ViewAccount& email) {
    bool result;
    openDatabase(msg);
    if (!db) return false;

    char const* sql = "SELECT * FROM view_email WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stmtToViewAccount(stmt, email);
        msg = "查询成功";
        result = true;
    }
    else {
        msg = "未找到对应记录";
        result = false;
    }

    sqlite3_finalize(stmt);
    closeDatabase();

    return result;
}

// 查询所有 email 记录
bool AccountDAO::findAllEmail(std::string& msg, const std::string& sysUserId, std::vector<ViewAccount>& emails) {
    openDatabase(msg);
    if (!db) return false;

    const std::string sql = "SELECT * FROM view_email WHERE sys_user_id = \'" + sysUserId + "\';";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        ViewAccount email;
        stmtToViewAccount(stmt, email);
        emails.push_back(email);
    }

    msg = "查询成功";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}
