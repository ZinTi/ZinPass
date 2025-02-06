#include "CategoryDAO.h"

CategoryDAO::CategoryDAO(const std::string& dbPath) : dbPath(dbPath) {
    db = nullptr;
}

CategoryDAO::~CategoryDAO() = default;

void CategoryDAO::openDatabase(std::string& msg) {
    if (sqlite3_open(dbPath.c_str(), &this->db) != SQLITE_OK) {
        msg = "Failed to open database: " + std::string(sqlite3_errmsg(this->db));
    }
    else {
        msg = "Database opened successfully.";
    }
}

void CategoryDAO::closeDatabase() const {
    sqlite3_close(this->db);
}

bool CategoryDAO::categoryNameToId(std::string& msg, const std::string& categoryName, short* id){
    bool result;
    openDatabase(msg);
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, "SELECT id FROM account_category WHERE name = ?", -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "查询失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    sqlite3_bind_text(stmt, 1, categoryName.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        *id = static_cast<short>(sqlite3_column_int(stmt, 0));
        msg = "查询成功";
        result = true;
    }else {
        *id = -1;
        msg = "未找到指定类别的id, 赋值 -1";
        result = false;
    }
    sqlite3_finalize(stmt);
    closeDatabase();
    return result;
}

bool CategoryDAO::findAllCategories(std::string& msg, std::vector<std::string>& categories) {
    openDatabase(msg);
    if (!db) return false;

    const std::string sql = "SELECT name FROM account_category";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        msg = "SQL语句准备失败: " + std::string(sqlite3_errmsg(db));
        closeDatabase();
        return false;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        categories.emplace_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }

    msg = "查询到类别数：" + std::to_string(categories.size());
    sqlite3_finalize(stmt);
    closeDatabase();
    return true;
}
