#include "TelecomOperatorDAO.h"

TelecomOperatorDAO::TelecomOperatorDAO(const std::string& dbPath) : dbPath(dbPath){
    this->db = nullptr;
}
TelecomOperatorDAO::~TelecomOperatorDAO(){
}
void TelecomOperatorDAO::openDatabase(std::string& msg){
    if (sqlite3_open(dbPath.c_str(), &this->db) != SQLITE_OK) {
        msg = "Failed to open database: " + std::string(sqlite3_errmsg(this->db));
    }
    else {
        msg = "Database opened successfully.";
    }
}
void TelecomOperatorDAO::closeDatabase() const{
    sqlite3_close(this->db);
}

bool TelecomOperatorDAO::findList(std::string& msg, std::vector<std::string>& telecomOperators){
    openDatabase(msg);
    if (!db) return false;

    const char* sql = "SELECT name FROM telecom_operator ORDER BY id ASC";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        telecomOperators.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    msg = "查询到电信运营商数量：" + std::to_string(telecomOperators.size());
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}


