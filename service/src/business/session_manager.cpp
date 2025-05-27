#include "session_manager.h"
#include "date_time.h"
#include "session_dao.h"
#include "uuid_generator.h"

namespace zinpass::business {
    SessionManager::SessionManager() = default;
    SessionManager::~SessionManager() = default;

    std::string SessionManager::create_session(const std::string& sys_user_id, const int timeout_seconds) {
        std::string session_id = utils::UUIDGenerator::generate();
        const std::string current_date_time = utils::DateTime::getCurrentDateTime();
        const std::string expires_date_time = utils::DateTime::addSeconds(current_date_time, timeout_seconds);

        const models::Session session(
            static_cast<const std::string>(session_id),
            sys_user_id,
            "default",
            current_date_time,
            expires_date_time,
            current_date_time
        );
        const repository::SessionDAO session_dao;
        if (const repository::DaoStatus ret = session_dao.add(session);
            repository::DaoStatus::Success != ret) {
            return "";
        }
        return session_id;
    }

    bool SessionManager::destroy_session(const std::string& session_id){
        if (const repository::SessionDAO session_dao;
            repository::DaoStatus::Success != session_dao.remove(session_id)) {
            return false;
        }
        return true;
    }

    bool SessionManager::update_session_expiration(const std::string& session_id, const std::string& new_expires_time) {
        const repository::SessionDAO session_dao;
        if (repository::DaoStatus::Success != session_dao.update_expiration(session_id, new_expires_time)) {
            return false;
        }
        return true;
    }

    bool SessionManager::renew_session_expiration(const std::string& session_id, const int minutes){
        const repository::SessionDAO session_dao;
        models::Session session;
        if (repository::DaoStatus::Success != session_dao.find_by_id(session_id, session)) {
            return false;
        }
        const std::string new_expires_time = utils::DateTime::addMinutes(session.getExpiresTime(), minutes);
        if (repository::DaoStatus::Success != session_dao.update_expiration(session_id, new_expires_time)) {
            return false;
        }
        return true;
    }

    // private
    bool SessionManager::renew_session_expiration(const std::string& session_id, const std::string& old_expires_time, const int minutes) {
        const std::string new_expires_time = utils::DateTime::addMinutes(old_expires_time, minutes);
        if (const repository::SessionDAO session_dao;
            repository::DaoStatus::Success == session_dao.update_expiration(session_id, new_expires_time)) {
            return true;
            }
        return false;
    }

    std::string SessionManager::get_session_attributes(const std::string& session_id){
        return "此功能未实现";
    }

    void SessionManager::set_session_attributes(const std::string& session_id, const std::string& data){
        // "此功能未实现"
    }

    bool SessionManager::validate_session(const std::string& session_id){
        models::Session session;
        const repository::SessionDAO session_dao;
        if (const repository::DaoStatus ret1 = session_dao.find_by_id(session_id, session);
            repository::DaoStatus::Success != ret1) {
            return false;
        }
        if(0 >= utils::DateTime::compareDateTimes(session.getExpiresTime(),utils::DateTime::getCurrentDateTime())){
            return false;
        }
        return true;
    }

    std::string SessionManager::validate_session_and_get_user_id(const std::string& session_id) {
        models::Session session;
        const repository::SessionDAO session_dao;
        if (const repository::DaoStatus ret1 = session_dao.find_by_id(session_id, session);
            repository::DaoStatus::Success != ret1){
            return "";
        }

        if(0 >= utils::DateTime::compareDateTimes(session.getExpiresTime(),utils::DateTime::getCurrentDateTime())){
            return "";
        }

        return session.getSysUserId();
    }

    std::string SessionManager::validate_session_and_get_user_id(const std::string& session_id, const bool renew) {
        models::Session session;
        const repository::SessionDAO session_dao;
        if (const repository::DaoStatus ret1 = session_dao.find_by_id(session_id, session);
            repository::DaoStatus::Success != ret1) {
            return "";
            }

        if(0 >= utils::DateTime::compareDateTimes(session.getExpiresTime(),utils::DateTime::getCurrentDateTime())){
            return "";
        }

        if (renew) {
            constexpr int minutes = 5;
            renew_session_expiration(session_id, session.getExpiresTime(), minutes); // 自动延长 session 有效期 5min
        }
        return session.getSysUserId();
    }

    bool SessionManager::is_session_expired(const std::string& session_id){
        return false;
    }

    int SessionManager::purge_expired_sessions(){
        int sum = 0;
        const repository::SessionDAO session_dao;
        const std::string now = utils::DateTime::getCurrentDateTime();
        const auto unordered_map = session_dao.find_id_and_expire();

        // 遍历键值对，删除无效 session
        for (const auto&[fst, snd] : unordered_map) {
            if (0 >= utils::DateTime::compareDateTimes(snd, now)) {
                if (repository::DaoStatus::Success == session_dao.remove(fst))  sum++;
            }
        }
        return sum;
    }

    std::string SessionManager::regenerate_session_id(const std::string& old_session_id){
        return "";
    }

    bool SessionManager::save_session(const std::string& session_id){
        return false;
    }

    models::Session SessionManager::load_session(const std::string& session_id){
        return {};
    }

    int SessionManager::get_active_session_count(){
        return -1;
    }

    std::vector<models::Session> SessionManager::list_all_sessions(){
        std::vector<models::Session> sessions;
        return sessions;
    }


}