#include "MobilePhone.h"

#include <utility>

// 默认构造函数
MobilePhone::MobilePhone() {
    this->id = 0;
    this->phoneNumber = "";
    this->telecomOperator = "";
    this->servicePwd = "";
    this->pin = "";
    this->puk = "";
    this->joinTime = "";
    this->phoneArea = "";
    this->sysUserId = "";
    this->createdTime = "";
    this->updatedTime = "";
}

// 带参数的构造函数
MobilePhone::MobilePhone(
    short id,
    std::string phoneNumber,
    std::string telecomOperator,
    std::string servicePwd,
    std::string pin,
    std::string puk,
    std::string joinTime,
    std::string phoneArea,
    std::string sysUserId,
    std::string createdTime,
    std::string updatedTime
) : id(id),
    phoneNumber(std::move(phoneNumber)),
    telecomOperator(std::move(telecomOperator)),
    servicePwd(std::move(servicePwd)),
    pin(std::move(pin)),
    puk(std::move(puk)),
    joinTime(std::move(joinTime)),
    phoneArea(std::move(phoneArea)),
    sysUserId(std::move(sysUserId)),
    createdTime(std::move(createdTime)),
    updatedTime(std::move(updatedTime)) {
}

// 拷贝构造函数
MobilePhone::MobilePhone(const MobilePhone& mobilePhone) {
    this->id = mobilePhone.id;
    this->phoneNumber = mobilePhone.phoneNumber;
    this->telecomOperator = mobilePhone.telecomOperator;
    this->servicePwd = mobilePhone.servicePwd;
    this->pin = mobilePhone.pin;
    this->puk = mobilePhone.puk;
    this->joinTime = mobilePhone.joinTime;
    this->phoneArea = mobilePhone.phoneArea;
    this->sysUserId = mobilePhone.sysUserId;
    this->createdTime = mobilePhone.createdTime;
    this->updatedTime = mobilePhone.updatedTime;
}

// 获取 id 的函数
short MobilePhone::getId() const {
    return id;
}

// 获取手机号码的函数
std::string MobilePhone::getPhoneNumber() const {
    return phoneNumber;
}

// 获取电信运营商的函数
std::string MobilePhone::getTelecomOperator() const {
    return telecomOperator;
}

// 获取服务密码的函数
std::string MobilePhone::getServicePwd() const {
    return servicePwd;
}

// 获取个人识别码的函数
std::string MobilePhone::getPin() const {
    return pin;
}

// 获取个人解锁码的函数
std::string MobilePhone::getPuk() const {
    return puk;
}

// 获取注册时间的函数
std::string MobilePhone::getJoinTime() const {
    return joinTime;
}

// 获取归属地的函数
std::string MobilePhone::getPhoneArea() const {
    return phoneArea;
}

std::string MobilePhone::getSysUserId() const {
    return sysUserId;
}

std::string MobilePhone::getCreatedTime() const {
    return createdTime;
}

std::string MobilePhone::getUpdatedTime() const {
    return updatedTime;
}

// 设置 id 的函数
void MobilePhone::setId(short id) {
    this->id = id;
}

// 设置手机号码的函数
void MobilePhone::setPhoneNumber(const std::string& phoneNumber) {
    this->phoneNumber = phoneNumber;
}

// 设置电信运营商的函数
void MobilePhone::setTelecomOperator(const std::string& phoneOperator) {
    this->telecomOperator = phoneOperator;
}

// 设置服务密码的函数
void MobilePhone::setServicePwd(const std::string& password) {
    this->servicePwd = password;
}

// 设置个人识别码的函数
void MobilePhone::setPin(const std::string& pin) {
    this->pin = pin;
}

// 设置个人解锁码的函数
void MobilePhone::setPuk(const std::string& puk) {
    this->puk = puk;
}

// 设置注册时间的函数
void MobilePhone::setJoinTime(const std::string& joinTime) {
    this->joinTime = joinTime;
}

// 设置归属地的函数
void MobilePhone::setPhoneArea(const std::string& phoneArea) {
    this->phoneArea = phoneArea;
}

void MobilePhone::setSysUserId(const std::string& sysUserId) {
    this->sysUserId = sysUserId;
}

void MobilePhone::setCreatedTime(const std::string& createdTime) {
    this->createdTime = createdTime;
}

void MobilePhone::setUpdatedTime(const std::string& updatedTime) {
    this->updatedTime = updatedTime;
}
