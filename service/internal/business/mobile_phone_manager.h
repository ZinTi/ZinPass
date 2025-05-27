#ifndef MOBILE_PHONE_MANAGER_H
#define MOBILE_PHONE_MANAGER_H

#include <string>
#include <vector>
#include "mobile_phone.h"

namespace zinpass::business{

class MobilePhoneManager {
public:
    template<typename T>
    struct Return {
        T value;
        std::string message;
    };

    /**
     * @brief get_phone_options
     * @param sys_user_id 手机号所属用户
     * @return phone_numbers + message
     */
    static Return<std::vector<std::string>> get_phone_options(const std::string& sys_user_id);

    /**
     * @brief get_mobile_phones
     * @param sys_user_id 手机号所属用户
     * @param telecom_operator 电信运营商（可选筛选条件）
     * @param phone_number 手机号（可选筛选条件）
     * @return phones + message
     */
    static Return<std::vector<models::MobilePhone>> get_mobile_phones(const std::string& sys_user_id, const std::string& telecom_operator, const std::string& phone_number);

    /**
     * @brief get_phone_by_owner_and_id 这个方法提供给普通用户查询，只能查询自己的手机号，不能查询其他用户的手机号
     * @param sys_user_id 手机号所属用户
     * @param phone_id 手机号 ID
     * @return phone + message
     */
    static Return<models::MobilePhone> get_phone_by_owner_and_id(const std::string& sys_user_id, int phone_id);

    /**
     * @brief get_phone_by_id 这个方法提供给管理员用户查询，查询指定 id 的手机号
     * @param phone_id 手机号 ID
     * @return phone + message
     */
    static Return<models::MobilePhone> get_phone_by_id(int phone_id);

    /**
     * @brief add_mobile_phone
     * @param sys_user_id 手机号所属用户
     * @param phone 手机号
     * @param telecom_operator 电信运营商
     * @param service_pwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param join_time 入网时间
     * @param phone_area 归属地
     * @param postscript 备注（附言）
     * @return 成功与否 + message
     */
    static Return<bool> add_mobile_phone(
        const std::string& sys_user_id,
        const std::string& phone,
        const std::string& telecom_operator,
        const std::string& service_pwd,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript);

    /**
     * @brief update_mobile_phone
     * @param id id
     * @param owner_id 记录所属人
     * @param phone_number 手机号
     * @param telecom_operator 电信运营商
     * @param service_pwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param join_time 入网时间
     * @param phone_area 归属地
     * @param postscript 备注
     * @return 成功与否 + message
     */
    static Return<bool> update_mobile_phone(
        int id,
        const std::string& owner_id,
        const std::string& phone_number,
        const std::string& telecom_operator,
        const std::string& service_pwd,
        const std::string& pin,
        const std::string& puk,
        const std::string& join_time,
        const std::string& phone_area,
        const std::string& postscript);

    /**
     * @brief delete_mobile_phone
     * @param id 删除目标
     * @param user_id 用户ID（如果目标所属user_id与此id不一致，则不予删除）
     * @return 成功与否 + message
     */
    static Return<bool> delete_mobile_phone(int id, const std::string& user_id);
};

}

#endif