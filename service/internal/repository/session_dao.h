#ifndef SESSION_DAO_H
#define SESSION_DAO_H
#include <string>
#include <map>
#include <unordered_map>
#include <vector>
#include "common/dao_status.h"
#include "session.h"
#include "connection_pool.h"
extern "C" {
    #include <sqlite3.h>
}

namespace zinpass::repository {

class SessionDAO {
public:
    explicit SessionDAO();
    ~SessionDAO();

    /**
     * @brief 根据 session_id 查询目标 session
     * @param id session_id
     * @param session 查询到的 session 信息
     * @return 执行结果
     */
    DaoStatus find_by_id(const std::string& id, models::Session &session) const;

    /**
     * @brief 查询所有 session 数据的 id 和 过期时间
     * @return session_id 和 过期时间的无序键值对
     */
    std::unordered_map<std::string, std::string> find_id_and_expire() const;

    DaoStatus add(const models::Session& session) const;

    DaoStatus update(const models::Session& session) const;

    DaoStatus remove(const std::string &id) const;

    /**
     * @brief 刷新会话的过期时间
     * @param id session_id
     * @param expires_date_time 过期时间
     * @return 执行结果
     */
    DaoStatus update_expiration(const std::string& id, const std::string& expires_date_time) const;

private:
    ConnectionPool& pool_;
    
};

}

#endif //SESSION_DAO_H
