#include "mobile_phone_dao.h"
#include "common/sql_debug.h"
#include "log_manager.h"
#include <sstream>
extern "C" {
#include <sqlite3.h>
}

namespace zinpass::repository{

MobilePhoneDAO::MobilePhoneDAO() : pool_(ConnectionPool::get_instance()) {
}

MobilePhoneDAO::~MobilePhoneDAO() = default;

models::MobilePhone MobilePhoneDAO::MobilePhoneFromStatement(sqlite3_stmt* stmt) {
    models::MobilePhone mobile_phone;

    if (SQLITE_NULL != sqlite3_column_type(stmt, 0)) {
        mobile_phone.setId(sqlite3_column_int(stmt, 0));
    }else {
        utils::LogManager::AddLog("[WARNING] Severe Warning: The primary key of the mobile_phone record is NULL, which may indicate an error. Resolution: Assign a default value of 0 to the variable.");
        mobile_phone.setId(0);
    }

    const auto* phone_number = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 1));
    mobile_phone.setPhoneNumber(phone_number ? phone_number : "");

    const auto* telecom_operator = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 2));
    mobile_phone.setTelecomOperator(telecom_operator ? telecom_operator : "");

    const auto* service_password = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 3));
    mobile_phone.setServicePwd(service_password ? service_password : "");

    const auto* pin = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 4));
    mobile_phone.setPin(pin ? pin : "");

    const auto* puk = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 5));
    mobile_phone.setPuk(puk ? puk : "");

    const auto* join_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 6));
    mobile_phone.setJoinTime(join_time ? join_time : "");

    const auto* phone_area = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 7));
    mobile_phone.setPhoneArea(phone_area ? phone_area : "");

    const auto* postscript = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 8));
    mobile_phone.setPostscript(postscript ? postscript : "");

    const auto* sys_user_id = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 9));
    mobile_phone.setSysUserId(sys_user_id ? sys_user_id : "");

    const auto* created_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 10));
    mobile_phone.setCreatedTime(created_time ? created_time : "");

    const auto* updated_time = reinterpret_cast<char const*>(sqlite3_column_text(stmt, 11));
    mobile_phone.setUpdatedTime(updated_time ? updated_time : "");
    return mobile_phone; // 编译器会自动优化调用拷贝构造函数返回对象的副本
}

int MobilePhoneDAO::generate_id()const {
    sqlite3* conn = pool_.get_connection();

    sqlite3_stmt* stmt = nullptr;

    const std::string sql_find_unused = "SELECT MIN(id) AS first_unused_id FROM (SELECT t1.id + 1 AS id FROM mobile_phone t1 LEFT JOIN mobile_phone t2 ON t1.id + 1 = t2.id WHERE t2.id IS NULL AND t1.id < (SELECT MAX(id) FROM mobile_phone) UNION SELECT (SELECT MAX(id) FROM mobile_phone) + 1 AS id) AS subquery;";
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql_find_unused.c_str(), sql_find_unused.size(), &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("Prepare failed: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return -1;
    }
    int id = -1;
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW == sqlite3_step(stmt)) {
        id = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return id;
}

DaoStatus MobilePhoneDAO::find_by_id(const int id, models::MobilePhone& mobile_phone) const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "SELECT id,phone_number,telecom_operator,service_pwd,pin,puk,join_time,phone_area,postscript,sys_user_id,created_time,updated_time FROM mobile_phone WHERE id = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_int(stmt, 1, id);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Phone not found with the given ID in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    }

    mobile_phone = MobilePhoneFromStatement(stmt);  // 确保列顺序一致
    utils::LogManager::AddLog("[OK] Mobile phone found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus MobilePhoneDAO::find_by_number(const std::string& phone_number, models::MobilePhone& mobile_phone)const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "SELECT id,phone_number,telecom_operator,service_pwd,pin,puk,join_time,phone_area,postscript,sys_user_id,created_time,updated_time FROM mobile_phone WHERE phone_number = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, phone_number.c_str(), static_cast<int>(phone_number.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[NOT FOUND] Phone not found with the given phone_number in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::NotFound;
    }

    mobile_phone = MobilePhoneFromStatement(stmt);
    utils::LogManager::AddLog("[OK] Mobile phone found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

int MobilePhoneDAO::find_id_by_number(const std::string& phone_number) const {
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id FROM mobile_phone WHERE phone_number = ?";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return -2;
    }
    sqlite3_bind_text(stmt, 1, phone_number.c_str(), static_cast<int>(phone_number.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    if (SQLITE_ROW != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(
            std::string("[NOT FOUND] Phone id not found with the given phone_number in the database."));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return -1;
    }

    const int id = sqlite3_column_int(stmt, 0);
    utils::LogManager::AddLog("[OK] Phone id found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return id;
}

DaoStatus MobilePhoneDAO::find_all(
    const std::string& sys_user_id,
    const std::string& telecom_operator,
    const std::string& phone_number,
    std::vector<models::MobilePhone>& mobile_phones) const
{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT id,phone_number,telecom_operator,service_pwd,pin,puk,join_time,phone_area,postscript,sys_user_id,created_time,updated_time FROM mobile_phone WHERE sys_user_id = ?1 AND phone_number LIKE ?2 AND (telecom_operator LIKE ?3 OR (?4 = '' AND telecom_operator IS NULL))";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);

    std::ostringstream oss_phone_number;
    oss_phone_number << "%" << phone_number << "%";
    sqlite3_bind_text(stmt, 2, oss_phone_number.str().c_str(), static_cast<int>(oss_phone_number.str().size()), SQLITE_TRANSIENT);  // 不要用SQLITE_TRANSIENT

    std::ostringstream oss_telecom_operator;
    oss_telecom_operator << "%" << telecom_operator << "%";
    sqlite3_bind_text(stmt, 3, oss_telecom_operator.str().c_str(), static_cast<int>(oss_telecom_operator.str().size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, telecom_operator.c_str(), static_cast<int>(telecom_operator.size()), SQLITE_TRANSIENT);

    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        mobile_phones.push_back(MobilePhoneFromStatement(stmt));
    }
    utils::LogManager::AddLog("[OK] Phone found.");
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus MobilePhoneDAO::find_list(const std::string& sys_user_id, std::vector<std::string>& phone_numbers)const{
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "SELECT phone_number FROM mobile_phone WHERE sys_user_id = ?";
    sqlite3_stmt* stmt = nullptr;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_text(stmt, 1, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    while (SQLITE_ROW == sqlite3_step(stmt)) {
        phone_numbers.emplace_back(reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)));
    }

    utils::LogManager::AddLog(std::string("[OK] Number of MobilePhone records found: ") + std::to_string(phone_numbers.size()));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus MobilePhoneDAO::add(const models::MobilePhone& mobile_phone)const {
    sqlite3* conn = pool_.get_connection();
    const std::string sql = "INSERT INTO mobile_phone(id, phone_number, telecom_operator, service_pwd, pin, puk, join_time, phone_area, postscript, sys_user_id, created_time, updated_time) "
                            "VALUES (?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9, ?10, ?11, ?12)";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    const int primaryKey = generate_id();
    sqlite3_bind_int(stmt, 1, primaryKey);
    if (mobile_phone.getPhoneNumber().empty()) sqlite3_bind_null(stmt, 2);
    else sqlite3_bind_text(stmt, 2, mobile_phone.getPhoneNumber().c_str(), static_cast<int>(mobile_phone.getPhoneNumber().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getTelecomOperator().empty()) sqlite3_bind_null(stmt, 3);
    else sqlite3_bind_text(stmt, 3, mobile_phone.getTelecomOperator().c_str(), static_cast<int>(mobile_phone.getTelecomOperator().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getServicePwd().empty()) sqlite3_bind_null(stmt, 4);
    else sqlite3_bind_text(stmt, 4, mobile_phone.getServicePwd().c_str(), static_cast<int>(mobile_phone.getServicePwd().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getPin().empty()) sqlite3_bind_null(stmt, 5);
    else sqlite3_bind_text(stmt, 5, mobile_phone.getPin().c_str(), static_cast<int>(mobile_phone.getPin().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getPuk().empty()) sqlite3_bind_null(stmt, 6);
    else sqlite3_bind_text(stmt, 6, mobile_phone.getPuk().c_str(), static_cast<int>(mobile_phone.getPuk().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getJoinTime().empty()) sqlite3_bind_null(stmt, 7);
    else sqlite3_bind_text(stmt, 7, mobile_phone.getJoinTime().c_str(), static_cast<int>(mobile_phone.getJoinTime().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getPhoneArea().empty()) sqlite3_bind_null(stmt, 8);
    else sqlite3_bind_text(stmt, 8, mobile_phone.getPhoneArea().c_str(), static_cast<int>(mobile_phone.getPhoneArea().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getPostscript().empty()) sqlite3_bind_null(stmt, 9);
    else sqlite3_bind_text(stmt, 9, mobile_phone.getPostscript().c_str(), static_cast<int>(mobile_phone.getPostscript().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getSysUserId().empty()) sqlite3_bind_null(stmt, 10);
    sqlite3_bind_text(stmt,10, mobile_phone.getSysUserId().c_str(), static_cast<int>(mobile_phone.getSysUserId().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getCreatedTime().empty()) sqlite3_bind_null(stmt, 11);
    else sqlite3_bind_text(stmt, 11, mobile_phone.getCreatedTime().c_str(), static_cast<int>(mobile_phone.getCreatedTime().size()), SQLITE_TRANSIENT);
    if (mobile_phone.getUpdatedTime().empty()) sqlite3_bind_null(stmt, 12);
    else sqlite3_bind_text(stmt, 12, mobile_phone.getUpdatedTime().c_str(), static_cast<int>(mobile_phone.getUpdatedTime().size()), SQLITE_TRANSIENT);
    SQLDebug::log_sql(stmt, true);
    DaoStatus status;
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to insert mobile_phone record: ") + std::string(sqlite3_errmsg(conn)));
        status = DaoStatus::DuplicateKey;
    }else {
        utils::LogManager::AddLog(std::string("[OK] New mobile phone created successfully: ") + std::to_string(primaryKey));
        status = DaoStatus::Success;
    }
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return status;
}

DaoStatus MobilePhoneDAO::update(
    const int id,
    const std::string& sys_user_id,
    const std::string& phone_number,
    const std::string& telecom_operator,
    const std::string& service_password,
    const std::string& pin,
    const std::string& puk,
    const std::string& join_time,
    const std::string& phone_area,
    const std::string& postscript,
    const std::string& updated_time) const {
    if (id < 0) return DaoStatus::InvalidData;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE mobile_phone SET phone_number=?1,telecom_operator=?2,service_pwd=?3,pin=?4,puk=?5,"
        "join_time=?6,phone_area=?7,postscript=?8,updated_time=?9 WHERE id=?10 AND sys_user_id=?11";

    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (phone_number.empty()) { sqlite3_bind_null(stmt, 1); }
    else { sqlite3_bind_text(stmt, 1, phone_number.c_str(), static_cast<int>(phone_number.size()), SQLITE_TRANSIENT); }
    if (telecom_operator.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, telecom_operator.c_str(), static_cast<int>(telecom_operator.size()), SQLITE_TRANSIENT); }
    if (service_password.empty()) { sqlite3_bind_null(stmt, 3); }
    else { sqlite3_bind_text(stmt, 3, service_password.c_str(), static_cast<int>(service_password.size()), SQLITE_TRANSIENT); }
    if (pin.empty()) { sqlite3_bind_null(stmt, 4); }
    else { sqlite3_bind_text(stmt, 4, pin.c_str(), static_cast<int>(pin.size()), SQLITE_TRANSIENT); }
    if (puk.empty()) { sqlite3_bind_null(stmt, 5); }
    else { sqlite3_bind_text(stmt, 5, puk.c_str(), static_cast<int>(puk.size()), SQLITE_TRANSIENT); }
    if (join_time.empty()) { sqlite3_bind_null(stmt, 6); }
    else { sqlite3_bind_text(stmt, 6, join_time.c_str(), static_cast<int>(join_time.size()), SQLITE_TRANSIENT); }
    if (phone_area.empty()) { sqlite3_bind_null(stmt, 7); }
    else { sqlite3_bind_text(stmt, 7, phone_area.c_str(), static_cast<int>(phone_area.size()), SQLITE_TRANSIENT); }
    if (postscript.empty()) { sqlite3_bind_null(stmt, 8); }
    else { sqlite3_bind_text(stmt, 8, postscript.c_str(), static_cast<int>(postscript.size()), SQLITE_TRANSIENT); }
    if (updated_time.empty()) { sqlite3_bind_null(stmt, 9); }
    else { sqlite3_bind_text(stmt, 9, updated_time.c_str(), static_cast<int>(updated_time.size()), SQLITE_TRANSIENT); }
    sqlite3_bind_int(stmt, 10, id);
    if (sys_user_id.empty()) { sqlite3_bind_null(stmt, 11); }
    else { sqlite3_bind_text(stmt, 11, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT); }

    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }
    utils::LogManager::AddLog(std::string("[OK] Phone updated successfully: ") + std::to_string(id));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

// 3/3
DaoStatus MobilePhoneDAO::update(
    const int id,
    const std::string& old_owner_id,
    const std::string& new_owner_id,
    const std::string& updated_time) const
{
    if (id < 0) return DaoStatus::InvalidData;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "UPDATE mobile_phone SET sys_user_id=?1, updated_time=?2 WHERE id=?3 AND sys_user_id=?4";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    if (new_owner_id.empty()) { sqlite3_bind_null(stmt, 1); }
    else { sqlite3_bind_text(stmt, 1, new_owner_id.c_str(), static_cast<int>(new_owner_id.size()), SQLITE_TRANSIENT); }
    if (updated_time.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, updated_time.c_str(), static_cast<int>(updated_time.size()), SQLITE_TRANSIENT); }
    sqlite3_bind_int(stmt, 3, id);
    if (old_owner_id.empty()) { sqlite3_bind_null(stmt, 4); }
    else { sqlite3_bind_text(stmt, 4, old_owner_id.c_str(), static_cast<int>(old_owner_id.size()), SQLITE_TRANSIENT); }

    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to update record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::GenericError;
    }
    utils::LogManager::AddLog(std::string("[OK] Phone updated successfully: ") + std::to_string(id));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

DaoStatus MobilePhoneDAO::remove(const int id, const std::string& sys_user_id) const {
    if (id < 0) return DaoStatus::InvalidData;
    sqlite3* conn = pool_.get_connection();

    const std::string sql = "DELETE FROM mobile_phone WHERE id = ?1 AND sys_user_id = ?2";
    sqlite3_stmt* stmt;
    if (SQLITE_OK != sqlite3_prepare_v2(conn, sql.c_str(), -1, &stmt, nullptr)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to prepare statement: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }
    sqlite3_bind_int(stmt, 1, id);
    if (sys_user_id.empty()) { sqlite3_bind_null(stmt, 2); }
    else { sqlite3_bind_text(stmt, 2, sys_user_id.c_str(), static_cast<int>(sys_user_id.size()), SQLITE_TRANSIENT); }

    SQLDebug::log_sql(stmt, true);
    if (SQLITE_DONE != sqlite3_step(stmt)) {
        utils::LogManager::AddLog(std::string("[ERROR] Failed to remove record: ") + std::string(sqlite3_errmsg(conn)));
        sqlite3_finalize(stmt);
        pool_.release_connection(conn);
        return DaoStatus::InvalidData;
    }

    utils::LogManager::AddLog(std::string("[OK] Phone removed successfully: ") + std::to_string(id));
    sqlite3_finalize(stmt);
    pool_.release_connection(conn);
    return DaoStatus::Success;
}

}
