#include "models/mobile_phone.h"
#include "mgr/phone_mgr.h"
#include "repo/account_dao.h"
#include "utils/date_time.h"
#include "repo/phone_dao.h"

namespace zinpass::business{


MobilePhoneManager::Return<std::vector<std::string>> MobilePhoneManager::get_phone_options(const std::string& sys_user_id) {
    std::vector<std::string> phone_numbers;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_list(sys_user_id, phone_numbers);
        repo::DaoStatus::Success != ret_mobile_phone_dao ) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {phone_numbers, message};
}

MobilePhoneManager::Return<std::vector<models::MobilePhone>> MobilePhoneManager::get_mobile_phones(const std::string& sys_user_id, const std::string& telecom_operator, const std::string& phone_number) {
    std::vector<models::MobilePhone> phones;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_all(sys_user_id, telecom_operator, phone_number, phones);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {phones, message};
}

MobilePhoneManager::Return<models::MobilePhone> MobilePhoneManager::get_phone_by_owner_and_id(const std::string& sys_user_id, const int phone_id) {
    const models::MobilePhone phone;
    models::MobilePhone temp_phone;

    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_by_id(phone_id, temp_phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        return {phone, "发生错误"};
    }
    if (temp_phone.getSysUserId() != sys_user_id) {
        return {phone, "未查询到符合条件的记录"};  // 仅查询指定用户的手机号，避免其他用户通过 id 查询到
    }
    return {temp_phone, "成功"};
}

MobilePhoneManager::Return<models::MobilePhone> MobilePhoneManager::get_phone_by_id(const int phone_id) {
    models::MobilePhone phone;
    const repo::MobilePhoneDAO mobile_phone_dao;
    if (const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.find_by_id(phone_id, phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        return {phone, "发生错误"};
    }
    return {phone, "成功"};
}

MobilePhoneManager::Return<bool> MobilePhoneManager::add_mobile_phone(
    const std::string& sys_user_id,
    const std::string& phone,
    const std::string& telecom_operator,
    const std::string& service_pwd,
    const std::string& pin,
    const std::string& puk,
    const std::string& join_time,
    const std::string& phone_area,
    const std::string& postscript)
{
    bool ret;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;

    models::MobilePhone new_phone;
    new_phone.setPhoneNumber(phone);
    new_phone.setTelecomOperator(telecom_operator);
    new_phone.setServicePwd(service_pwd);
    new_phone.setPin(pin);
    new_phone.setPuk(puk);
    new_phone.setJoinTime(join_time);
    new_phone.setPhoneArea(phone_area);
    new_phone.setPostscript(postscript);
    // new_phone.setId();
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    new_phone.setCreatedTime(current_date_time);
    new_phone.setUpdatedTime(current_date_time);
    new_phone.setSysUserId(sys_user_id);

    if (
        const repo::DaoStatus ret_mobile_phone_dao = mobile_phone_dao.add(new_phone);
        repo::DaoStatus::Success != ret_mobile_phone_dao) {
        message = "发生了错误";
        ret = false;
    }else {
        message = "成功";
        ret = true;
    }

    return {ret, message};
}

MobilePhoneManager::Return<bool> MobilePhoneManager::update_mobile_phone(
    const int id,
    const std::string& owner_id,
    const std::string& phone_number,
    const std::string& telecom_operator,
    const std::string& service_pwd,
    const std::string& pin,
    const std::string& puk,
    const std::string& join_time,
    const std::string& phone_area,
    const std::string& postscript)
{
    bool ret;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();

    const repo::DaoStatus ret_update = mobile_phone_dao.update(id,owner_id,
        phone_number,telecom_operator,service_pwd,pin,puk,
        join_time,phone_area,postscript,current_date_time);

    if (
        repo::DaoStatus::Success != ret_update
    ) {
        message = "发生错误";
        ret = false;
    }else {
        message = "成功";
        ret = true;
    }
    return {ret, message};
}

MobilePhoneManager::Return<bool> MobilePhoneManager::delete_mobile_phone(const int id, const std::string& user_id) {
    bool ret;
    std::string message;
    const repo::MobilePhoneDAO mobile_phone_dao;
    const std::string current_date_time = utils::DateTime::getCurrentDateTime();
    if (
        const repo::DaoStatus ret_status = mobile_phone_dao.remove(id, user_id);
        repo::DaoStatus::Success != ret_status
    ) {
        message = "发生错误";
        ret = false;
    } else {
        message = "成功";
        ret = true;
    }
    return {ret, message};
}

}