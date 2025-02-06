#include "MobilePhoneDAO.h"
#include "sqlite3.h"
#include <stdexcept>

MobilePhoneDAO::MobilePhoneDAO(const std::string& dbPath) : dbPath(dbPath) {
    db = nullptr;
}

MobilePhoneDAO::~MobilePhoneDAO() = default;

void MobilePhoneDAO::openDatabase(std::string& msg) {
    if (sqlite3_open(dbPath.c_str(), &this->db) != SQLITE_OK) {
        msg = "无法打开数据库: " + std::string(sqlite3_errmsg(db));
        throw std::runtime_error(msg);
    }
}

void MobilePhoneDAO::closeDatabase() const {
    sqlite3_close(this->db);
}

void MobilePhoneDAO::stmtToMobilePhone(sqlite3_stmt* stmt, MobilePhone& mobilePhone) {
    mobilePhone.setId(static_cast<short>(sqlite3_column_int(stmt, 0)));

    const auto phoneN = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1));
    mobilePhone.setPhoneNumber(phoneN == nullptr ? "" : phoneN);

    const auto teleOperator = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2));
    mobilePhone.setTelecomOperator(teleOperator == nullptr ? "" : teleOperator);

    const auto servicePwd = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3));
    mobilePhone.setServicePwd(servicePwd == nullptr ? "" : servicePwd);

    const auto pin = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4));
    mobilePhone.setPin(pin == nullptr ? "" : pin);

    const auto puk = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5));
    mobilePhone.setPuk(puk == nullptr ? "" : puk);

    const auto joinTime = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 6));
    mobilePhone.setJoinTime(joinTime == nullptr ? "" : joinTime);

    const auto phoneArea = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 7));
    mobilePhone.setPhoneArea(phoneArea == nullptr ? "" : phoneArea);

    const auto owner = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 8));
    mobilePhone.setSysUserId(owner == nullptr ? "" : owner);

    const auto createdTime = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 9));
    mobilePhone.setCreatedTime(createdTime == nullptr ? "" : createdTime);

    const auto updatedTime = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 10));
    mobilePhone.setUpdatedTime(updatedTime == nullptr ? "" : updatedTime);
}

int MobilePhoneDAO::generateId(std::string& msg) {
    openDatabase(msg);
    if (msg.find("Failed") != std::string::npos) {
        return -1;
    }

    sqlite3_stmt* stmt = nullptr;
    int id = 0;

    // 2. 首先获取目前表中最大的 id
    const std::string sql_find_unused =
        "SELECT MIN(id) AS first_unused_id FROM (SELECT t1.id + 1 AS id FROM mobile_phone t1 LEFT JOIN mobile_phone t2 ON t1.id + 1 = t2.id WHERE t2.id IS NULL AND t1.id < (SELECT MAX(id) FROM mobile_phone) UNION SELECT (SELECT MAX(id) FROM mobile_phone) + 1 AS id) AS subquery;";
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

bool MobilePhoneDAO::findById(std::string& msg, const int id, MobilePhone& mobilePhone) {
    openDatabase(msg);
    const auto sql = "SELECT * FROM mobile_phone WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stmtToMobilePhone(stmt, mobilePhone); // 将stmt的数据复制到mobilePhone
        msg = "查询成功";
        sqlite3_finalize(stmt);
        closeDatabase();
        return true;
    }

    msg = "未找到指定id的数据";
    sqlite3_finalize(stmt);
    closeDatabase();
    return false;
}

bool MobilePhoneDAO::findByMobilePhone(std::string& msg, const std::string& phoneNumber, MobilePhone& mobilePhone) {
    openDatabase(msg);
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, "SELECT * FROM mobile_phone WHERE phone_number = ?", -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, phoneNumber.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stmtToMobilePhone(stmt, mobilePhone); // 将stmt的数据复制到mobilePhone
        msg = "查询成功";
        sqlite3_finalize(stmt);
        closeDatabase();
        return true;
    }

    msg = "未找到指定手机号的数据";
    sqlite3_finalize(stmt);
    closeDatabase();
    return false;
}

bool MobilePhoneDAO::phoneNumberToId(std::string& msg, const std::string& phoneNumber, short* id) {
    bool result;
    openDatabase(msg);
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, "SELECT id FROM mobile_phone WHERE phone_number = ?", -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, phoneNumber.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *id = static_cast<short>(sqlite3_column_int(stmt, 0));
        msg = "查询成功";
        result = true;
    }
    else {
        *id = -1;
        msg = "未找到指定手机号的id, 赋值 -1";
        result = false;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

bool MobilePhoneDAO::findAll(std::string& msg, const std::string& sysUserId, std::vector<MobilePhone>& mobilePhones) {
    openDatabase(msg);
    const std::string sql = "SELECT * FROM mobile_phone WHERE sys_user_id = \'" + sysUserId + "\';";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        MobilePhone mobilePhone;
        stmtToMobilePhone(stmt, mobilePhone); // 将stmt的数据复制到mobilePhone

        mobilePhones.push_back(mobilePhone);
    }

    msg = "查询成功";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool MobilePhoneDAO::findList(std::string& msg, const std::string& sysUserId,
                              std::vector<std::string>& phoneNumberList) {
    openDatabase(msg);
    if (!db) return false;

    const std::string sql = "SELECT phone_number FROM mobile_phone WHERE sys_user_id = \'" + sysUserId + "\'";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        phoneNumberList.emplace_back(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
    }

    msg = "查询到手机号数：" + std::to_string(phoneNumberList.size());
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool MobilePhoneDAO::add(std::string& msg, const MobilePhone& mobilePhone) {
    openDatabase(msg);
    char const* sql = "INSERT INTO mobile_phone VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "插入失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }
    std::string genIdMsg;
    const int primaryKey = generateId(genIdMsg);

    sqlite3_bind_int(stmt, 1, primaryKey);
    if (mobilePhone.getPhoneNumber().empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, mobilePhone.getPhoneNumber().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getTelecomOperator().empty()) { sqlite3_bind_null(stmt, 3); }
    else { sqlite3_bind_text(stmt, 3, mobilePhone.getTelecomOperator().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getServicePwd().empty()) { sqlite3_bind_null(stmt, 4); }
    else { sqlite3_bind_text(stmt, 4, mobilePhone.getServicePwd().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getPin().empty()) { sqlite3_bind_null(stmt, 5); }
    else { sqlite3_bind_text(stmt, 5, mobilePhone.getPin().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getPuk().empty()) { sqlite3_bind_null(stmt, 6); }
    else { sqlite3_bind_text(stmt, 6, mobilePhone.getPuk().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getJoinTime().empty()) { sqlite3_bind_null(stmt, 7); }
    else { sqlite3_bind_text(stmt, 7, mobilePhone.getJoinTime().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getPhoneArea().empty()) { sqlite3_bind_null(stmt, 8); }
    else { sqlite3_bind_text(stmt, 8, mobilePhone.getPhoneArea().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getSysUserId().empty()) { sqlite3_bind_null(stmt, 9); }
    else { sqlite3_bind_text(stmt, 9, mobilePhone.getSysUserId().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getCreatedTime().empty()) { sqlite3_bind_null(stmt, 10); }
    else { sqlite3_bind_text(stmt, 10, mobilePhone.getCreatedTime().c_str(), -1, SQLITE_TRANSIENT); }
    if (mobilePhone.getUpdatedTime().empty()) { sqlite3_bind_null(stmt, 11); }
    else { sqlite3_bind_text(stmt, 11, mobilePhone.getUpdatedTime().c_str(), -1, SQLITE_TRANSIENT); }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "插入失败: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "插入成功";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}

bool MobilePhoneDAO::update(std::string& msg, const short id, const std::string& updatedTime,
                            const std::string& phoneNumber, const std::string& servicePwd, const std::string& pin,
                            const std::string& puk) {
    bool result;
    openDatabase(msg);
    char const* sql =
        "UPDATE mobile_phone SET updated_time = ?, phone_number = ?, service_pwd = ?, pin = ?, puk = ? WHERE id = ?";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }
    if (updatedTime.empty()) { sqlite3_bind_null(stmt, 1); }
    else { sqlite3_bind_text(stmt, 1, updatedTime.c_str(), -1, SQLITE_TRANSIENT); }
    if (phoneNumber.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, phoneNumber.c_str(), -1, SQLITE_TRANSIENT); }
    if (servicePwd.empty()) { sqlite3_bind_null(stmt, 3); }
    else { sqlite3_bind_text(stmt, 3, servicePwd.c_str(), -1, SQLITE_TRANSIENT); }
    if (pin.empty()) { sqlite3_bind_null(stmt, 4); }
    else { sqlite3_bind_text(stmt, 4, pin.c_str(), -1, SQLITE_TRANSIENT); }
    if (puk.empty()) { sqlite3_bind_null(stmt, 5); }
    else { sqlite3_bind_text(stmt, 5, puk.c_str(), -1, SQLITE_TRANSIENT); }
    if (id < 0) { sqlite3_bind_null(stmt, 6); }
    else { sqlite3_bind_int(stmt, 6, id); }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        result = false;
    }
    else {
        msg = "更新成功";
        result = true;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

bool MobilePhoneDAO::update(std::string& msg, const short id, const std::string& updatedTime,
                            const std::string& telecomOperator, const std::string& joinTime,
                            const std::string& phoneArea) {
    bool result;
    openDatabase(msg);
    char const* sql =
        "UPDATE mobile_phone SET updated_time = ?, telecom_operator = ?, join_time = ?, phone_area = ? WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }
    if (updatedTime.empty()) { sqlite3_bind_null(stmt, 1); }
    else { sqlite3_bind_text(stmt, 1, updatedTime.c_str(), -1, SQLITE_TRANSIENT); }
    if (telecomOperator.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, telecomOperator.c_str(), -1, SQLITE_TRANSIENT); }
    if (joinTime.empty()) { sqlite3_bind_null(stmt, 3); }
    else { sqlite3_bind_text(stmt, 3, joinTime.c_str(), -1, SQLITE_TRANSIENT); }
    if (phoneArea.empty()) { sqlite3_bind_null(stmt, 4); }
    else { sqlite3_bind_text(stmt, 4, phoneArea.c_str(), -1, SQLITE_TRANSIENT); }
    if (id < 0) { sqlite3_bind_null(stmt, 5); }
    else { sqlite3_bind_int(stmt, 5, id); }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        result = false;
    }
    else {
        msg = "更新成功";
        result = true;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

bool MobilePhoneDAO::update(std::string& msg, const short id, const std::string& updatedTime,
                            const std::string& owner) {
    bool result;
    openDatabase(msg);
    char const* sql = "UPDATE mobile_phone SET sys_user_id = ?, updated_time = ? WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    if (owner.empty()) { sqlite3_bind_null(stmt, 1); }
    else { sqlite3_bind_text(stmt, 1, owner.c_str(), -1, SQLITE_TRANSIENT); }
    if (updatedTime.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, updatedTime.c_str(), -1, SQLITE_TRANSIENT); }
    if (id < 0) { sqlite3_bind_null(stmt, 3); }
    else { sqlite3_bind_int(stmt, 3, id); }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "更新失败: " + std::string(sqlite3_errmsg(db));
        result = false;
    }
    else {
        msg = "更新成功";
        result = true;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

bool MobilePhoneDAO::remove(std::string& msg, const short id) {
    openDatabase(msg);
    char const* sql = "DELETE FROM mobile_phone WHERE id = ?";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "删除失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        msg = "删除失败: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        closeDatabase();
        return false;
    }

    msg = "删除成功";
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}
