#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include <string>
#include <vector>
#include "session.h"

namespace zinpass::business {

class SessionManager {
public:

    SessionManager();
    ~SessionManager();

    // （一）会话生命周期管理
    /**
     * @brief 创建新会话，关联用户ID和初始数据
     * @param sys_user_id 会话主键
     * @param timeout_seconds 过期时间（单位：秒），默认 30 min
     * @return 唯一会话ID
     */
    static std::string create_session(const std::string &sys_user_id,int timeout_seconds = 1800);

    /**
     * @brief 根据会话ID主动销毁会话，清除相关数据。
     * @param session_id 会话主键
     * @return 完成与否
     */
    static bool destroy_session(const std::string &session_id);

    /**
     * @brief 更新会话到指定过期时间
     * @param session_id 会话主键
     * @param new_expires_time 新过期时间
     * @return
     */
    static bool update_session_expiration(const std::string& session_id, const std::string& new_expires_time);

    /**
     * @brief 刷新会话的过期时间，防止自动失效（如用户活跃时调用）。
     * @param session_id 会话主键
     * @param minutes 要延长的分钟数
     * @return 成功与否
     */
    static bool renew_session_expiration(const std::string &session_id, int minutes);

private:
    /**
     * @brief 私有方法，刷新会话的过期时间
     * @param session_id 会话主键
     * @param old_expires_time 原过期时间
     * @param minutes 增加的分钟数
     * @return 成功与否
     */
    static bool renew_session_expiration(const std::string &session_id,const std::string &old_expires_time, int minutes);

public:
    // （二）会话数据操作
    /**
     * @brief 获取会话中存储的用户数据（如偏好设置等）。
     * @param session_id 会话主键
     * @return 用户数据
     */
    static std::string get_session_attributes(const std::string& session_id);

    /**
     * @brief 更新或追加会话的自定义数据（如用户操作状态变更）。
     * @param session_id 会话主键
     * @param data 自定义数据
     */
    static void set_session_attributes(const std::string& session_id,const std::string &data);

    // （三）会话状态验证
    /**
     * @brief 检查会话是否有效（未过期且存在），用于权限控制。
     * @param session_id 会话主键
     * @return 有效与否
     */
    static bool validate_session(const std::string &session_id);

    /**
     * @brief 检查会话是否有效（未过期且存在）
     * @param session_id 会话主键
     * @return 若有效则返回 user_id
     */
    static std::string validate_session_and_get_user_id(const std::string &session_id);

    /**
     * @brief 检查会话是否有效（未过期且存在），如果有效则延长会话有效期（可选）并返回 user_id
     * @param session_id 会话主键
     * @param renew 是否延长有效期
     * @return 若有效则返回 user_id
     */
    static std::string validate_session_and_get_user_id(const std::string &session_id, bool renew);

    /**
     * @brief 单独检测会话是否已超时（与持久化存储的过期策略解耦）。
     * @param session_id 会话主键
     * @return 会话是否超时
     */
    static bool is_session_expired(const std::string &session_id);

    // （四）安全与清理
    /**
     * @brief 清理所有过期的会话（由定时任务调用）。
     * @return 清除数量
     */
    static int purge_expired_sessions();

    /**
     * @brief 生成新会话ID并迁移数据，防止会话固定攻击（安全关键操作）。
     * @param old_session_id 旧会话ID
     * @return 新会话ID
     */
    static std::string regenerate_session_id(const std::string &old_session_id);

    // （五）持久化：本程序中暂时用不到，因为截至目前，session 都是保存在sqlite中的，而不是内存中
    /**
     * @brief 将会话数据持久化到数据库或分布式存储（用于跨服务共享）。
     * @param session_id 会话主键
     * @return 成功与否
     */
    static bool save_session(const std::string& session_id);

    /**
     * @brief 从持久化存储中加载会话（如服务重启后恢复）。
     * @param session_id 会话主键
     * @return 会话数据
     */
    static models::Session load_session(const std::string& session_id);

    // （六）统计与监控
    /**
     * @brief 统计当前活跃会话总数，用于监控系统负载。
     * @return 活跃会话总数
     */
    static int get_active_session_count();

    /**
     * @brief 获取所有会话详情（调试或管理后台用，需注意性能）。
     * @return 所有会话数据
     */
    static std::vector<models::Session> list_all_sessions();

};

}

#endif
