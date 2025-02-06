#include "SystemUserDAO.h"
#include "sqlite3.h"

SystemUserDAO::SystemUserDAO(const std::string& dbPath) : dbPath(dbPath) {
    this->db = nullptr;
}

SystemUserDAO::~SystemUserDAO() = default;

void SystemUserDAO::openDatabase(std::string& msg) {
    int rc = sqlite3_open(dbPath.c_str(), &db);
    if (rc) {
        msg = "无法打开数据库: " + std::string(sqlite3_errmsg(this->db));
    }
}

void SystemUserDAO::closeDatabase() const {
    sqlite3_close(this->db);
}

bool SystemUserDAO::findByUserId(std::string& msg, const std::string& userId, SystemUser& sysUser) {
    openDatabase(msg);
    if (!msg.empty()) return false;

    const std::string sql =
        "SELECT user_id, alias, hashed_pwd, salt, created_time, updated_time FROM system_user WHERE user_id = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "SQL语句预处理失败: " + std::string(sqlite3_errmsg(this->db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        sysUser.setUserId(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
        sysUser.setAlias(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1)));
        sysUser.setPwd(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2)));
        sysUser.setSalt(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3)));
        sysUser.setCreatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4)));
        sysUser.setUpdatedTime(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5)));
        sqlite3_finalize(stmt);
        closeDatabase();
        msg = "找到用户" + sysUser.getUserId();
        return true;
    }
    else {
        msg = "没有找到用户" + userId;
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }
}

bool SystemUserDAO::findAllUserId(std::string& msg, std::vector<std::string>& userIdList) {
    openDatabase(msg);
    if (!db) return false;

    char const* sql = "SELECT user_id FROM system_user;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(this->db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        userIdList.emplace_back(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
    }

    msg = "查询到用户数：" + std::to_string(userIdList.size());
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool SystemUserDAO::add(std::string& msg, const SystemUser& sysUser) {
    openDatabase(msg);
    if (!msg.empty()) return false;

    const std::string sql = "INSERT INTO system_user(user_id, alias, hashed_pwd, salt, created_time, updated_time) "
        "VALUES (?, ?, ?, ?, ?, ?)";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "SQL语句预处理失败: " + std::string(sqlite3_errmsg(this->db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, sysUser.getUserId().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, sysUser.getAlias().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, sysUser.getPwd().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, sysUser.getSalt().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, sysUser.getCreatedTime().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, sysUser.getUpdatedTime().c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        msg = "插入数据失败: " + std::string(sqlite3_errmsg(this->db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    sqlite3_finalize(stmt);
    closeDatabase();
    msg = "新增用户" + sysUser.getUserId() + "成功";
    return true;
}

bool SystemUserDAO::update(std::string& msg, const SystemUser& sysUser) {
    openDatabase(msg);
    if (!msg.empty()) return false;

    const std::string sql =
        "UPDATE system_user SET alias = ?, hashed_pwd = ?, salt = ?, updated_time = ? WHERE user_id = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "SQL语句预处理失败: " + std::string(sqlite3_errmsg(this->db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, sysUser.getAlias().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, sysUser.getPwd().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, sysUser.getSalt().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, sysUser.getUpdatedTime().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, sysUser.getUserId().c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        msg = "更新数据失败: " + std::string(sqlite3_errmsg(this->db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    sqlite3_finalize(stmt);
    closeDatabase();
    msg = "更新用户" + sysUser.getUserId() + "数据成功";
    return true;
}

bool SystemUserDAO::remove(std::string& msg, const std::string& userId) {
    openDatabase(msg);
    if (!msg.empty()) return false;

    const std::string sql = "DELETE FROM system_user WHERE user_id = ?";

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        msg = "SQL语句预处理失败: " + std::string(sqlite3_errmsg(this->db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, userId.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        msg = "删除数据失败: " + std::string(sqlite3_errmsg(this->db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    sqlite3_finalize(stmt);
    closeDatabase();
    msg = "删除用户" + userId + "成功";
    return true;
}
