#ifndef MOBILEPHONESERVICE_H
#define MOBILEPHONESERVICE_H

#include <string>
#include <vector>
#include "MobilePhone.h"

class MobilePhoneService {
public:
    /**
     * @brief getPhoneOptions
     * @param msg 反馈信息
     * @param phoneNumber 获取到的手机号
     * @return 成功与否
     */
    bool getPhoneOptions(std::string& msg, std::vector<std::string>& phoneNumber);

    /**
     * @brief 获取所有移动手机号
     * @param msg 反馈信息
     * @param phoneNumber 手机号
     * @param telecomOperator 电信运营商
     * @param phones 获取到的手机号
     * @return 成功与否
     */
    bool getMobilePhones(std::string& msg, const std::string& phoneNumber, const std::string& telecomOperator, std::vector<MobilePhone>& phones);

    /**
     * @brief 新增手机号
     * @param msg 反馈信息
     * @param phone 手机号
     * @param telecomOperator 电信运营商
     * @param servicePwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param joinTime 入网时间
     * @param phoneArea 归属地
     * @param postscript 备注（附言）
     * @return 成功与否
     */
    bool addMobilePhone(std::string& msg, const std::string& phone, const std::string& telecomOperator, const std::string& servicePwd, const std::string& pin, const std::string& puk, const std::string& joinTime,const std::string& phoneArea,const std::string& postscript);

    /**
     * @brief 更新手机号信息
     * @param msg 反馈信息
     * @param id id
     * @param phoneNumber 手机号
     * @param telecomOperator 电信运营商
     * @param servicePwd 服务密码
     * @param pin PIN
     * @param puk PUK
     * @param joinTime 入网时间
     * @param phoneArea 归属地
     * @param owner 记录所属人
     * @return 成功与否
     */
    bool updateMobilePhone(std::string& msg, short id, const std::string& phoneNumber,const std::string& telecomOperator, const std::string& servicePwd, const std::string& pin,const std::string& puk,const std::string& joinTime, const std::string& phoneArea, const std::string& owner);

    /**
     * @brief 删除手机号
     * @param msg 反馈信息
     * @param id 删除目标
     * @return 成功与否
     */
    bool deleteMobilePhone(std::string& msg, short id);
};



#endif