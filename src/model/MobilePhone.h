#ifndef MOBILEPHONE_H
#define MOBILEPHONE_H

#include <string>

class MobilePhone {
private:
    short id; // id
    std::string phoneNumber; // 手机号码
    std::string telecomOperator; // 电信运营商
    std::string servicePwd; // 服务密码
    std::string pin; // 个人识别码（PIN）
    std::string puk; // 个人解锁码（PUK）
    std::string joinTime; // 注册时间
    std::string phoneArea; // 归属地
    std::string sysUserId; // 所属系统用户
    std::string createdTime;
    std::string updatedTime;

public:
    // 构造函数，默认构造函数，可能用于创建一个未初始化的对象
    MobilePhone();
    // 带参数的构造函数，用于初始化对象的各个属性
    MobilePhone(short id, std::string phoneNumber, std::string telecomOperator, std::string servicePwd, std::string pin, std::string puk, std::string joinTime, std::string phoneArea, std::string sysUserId, std::string createdTime, std::string updatedTime);
    // 拷贝构造函数，用于复制另一个 MobilePhone 对象的数据到当前对象
    MobilePhone(const MobilePhone& mobilePhone);
    [[nodiscard]] short getId() const;
    [[nodiscard]] std::string getPhoneNumber() const;
    [[nodiscard]] std::string getTelecomOperator() const;
    [[nodiscard]] std::string getServicePwd() const;
    [[nodiscard]] std::string getPin() const;
    [[nodiscard]] std::string getPuk() const;
    [[nodiscard]] std::string getJoinTime() const;
    [[nodiscard]] std::string getPhoneArea() const;
    [[nodiscard]] std::string getSysUserId() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getUpdatedTime() const;
    void setId(short id);
    void setPhoneNumber(const std::string& phoneNumber);
    void setTelecomOperator(const std::string& phoneOperator);
    void setServicePwd(const std::string& password);
    void setPin(const std::string& pin);
    void setPuk(const std::string& puk);
    void setJoinTime(const std::string& joinTime);
    void setPhoneArea(const std::string& phoneArea);
    void setSysUserId(const std::string& sysUserId);
    void setCreatedTime(const std::string& createdTime);
    void setUpdatedTime(const std::string& updatedTime);
};

#endif
