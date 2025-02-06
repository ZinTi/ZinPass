#include "MobilePhoneService.h"

#include <AccountDAO.h>
#include <datetime.h>

#include "MobilePhoneDAO.h"
#include "Environ.h"
#include "SystemUser.h"

extern Environ* ZinPassEnv;
extern SystemUser* currentSystemUser;

bool MobilePhoneService::getPhoneOptions(std::string& msg, std::vector<std::string>& phoneNumber) {
    bool result;
    if (
        MobilePhoneDAO mobilePhoneDAO(ZinPassEnv->getDbPath());
        mobilePhoneDAO.findList(msg, currentSystemUser->getUserId(), phoneNumber)
    ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool MobilePhoneService::getMobilePhones(std::string& msg, const std::string& phoneNumber, const std::string& telecomOperator, std::vector<MobilePhone>& phones) {
    bool result;
    MobilePhoneDAO mobilePhoneDAO(ZinPassEnv->getDbPath());

    if (
        mobilePhoneDAO.findAll(msg, currentSystemUser->getUserId(), phones)
        ) {
        result = true;
    }
    else {
        result = false;
    }
    return result;
}

bool MobilePhoneService::addMobilePhone(std::string& msg, const std::string& phone, const std::string& telecomOperator, const std::string& servicePwd, const std::string& pin, const std::string& puk, const std::string& joinTime,const std::string& phoneArea,const std::string& postscript) {
    bool result;
    MobilePhoneDAO mobilePhoneDAO(ZinPassEnv->getDbPath());

    MobilePhone updateData;
    updateData.setPhoneNumber(phone);
    updateData.setTelecomOperator(telecomOperator);
    updateData.setServicePwd(servicePwd);
    updateData.setPin(pin);
    updateData.setPuk(puk);
    updateData.setJoinTime(joinTime);
    updateData.setPhoneArea(phoneArea);
    // updateData.setPostscript(postscript);
    // updateData.setId();
    const std::string currentDateTime = getCurrentDateTime();
    updateData.setCreatedTime(currentDateTime);
    updateData.setUpdatedTime(currentDateTime);
    updateData.setSysUserId(currentSystemUser->getUserId());

    if (
        mobilePhoneDAO.add(msg, updateData)
    ) {
        result = true;
    }else {
        result = false;
    }
    msg = getCurrentDateTime() + msg;
    return result;
}

bool MobilePhoneService::updateMobilePhone(std::string& msg, const short id, const std::string& phoneNumber, const std::string& telecomOperator, const std::string& servicePwd, const std::string& pin, const std::string& puk, const std::string& joinTime, const std::string& phoneArea, const std::string& owner) {
    bool result;
    MobilePhoneDAO mobilePhoneDAO(ZinPassEnv->getDbPath());
    const std::string currentDateTime = getCurrentDateTime();
    if (
        mobilePhoneDAO.update(msg, id, currentDateTime, phoneNumber, servicePwd, pin, puk) &&
        mobilePhoneDAO.update(msg, id, currentDateTime, telecomOperator, joinTime, phoneArea) &&
        mobilePhoneDAO.update(msg, id, currentDateTime, owner)
    ) {
        result = true;
    }else {
        result = false;
    }
    msg = getCurrentDateTime() + msg;
    return result;
}

bool MobilePhoneService::deleteMobilePhone(std::string& msg, const short id) {
    bool result;
    MobilePhoneDAO mobilePhoneDAO(ZinPassEnv->getDbPath());
    const std::string currentDateTime = getCurrentDateTime();
    if (
        mobilePhoneDAO.remove(msg, id)
    ) {
        result = true;
    }else {
        result = false;
    }
    msg = getCurrentDateTime() + msg;
    return result;
}
