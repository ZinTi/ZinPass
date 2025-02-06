#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

class Account {
protected:
    int id;
    std::string userId;
    std::string alias;
    std::string subAccount;
    std::string postscript;
    std::string platformName;
    std::string providerName;
    std::string URL;
    std::string hotline;
    std::string sysUserId;
    std::string createdTime;
    std::string updatedTime;

private:
    // std::string password; // 原来的，改为 encrypted_pwd
    std::vector<unsigned char> encryptedPwd;
    std::vector<unsigned char> iv; // 初始化向量IV
    short phoneId;
    short emailId;
    short categoryId;

public:
    Account();
    Account(std::vector<unsigned char> encryptedPwd, std::vector<unsigned char> iv, short phoneId, short emailId,
            int id, std::string userId, std::string alias, std::string subAccount, std::string postscript, std::string platformName, std::string providerName,
            std::string URL, std::string hotline, short categoryId, std::string sysUserId, std::string createdTime, std::string updatedTime);
    Account(Account const& account);
    [[nodiscard]] const std::vector<unsigned char>& getEncryptedPwd() const;
    [[nodiscard]] const std::vector<unsigned char>& getIv() const;
    [[nodiscard]] short getPhoneId() const;
    [[nodiscard]] short getEmailId() const;

    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getUserId() const;
    [[nodiscard]] std::string getAlias() const;
    [[nodiscard]] std::string getSubAccount() const;
    [[nodiscard]] std::string getPostscript() const;
    [[nodiscard]] std::string getPlatformName() const;
    [[nodiscard]] std::string getProviderName() const;
    [[nodiscard]] std::string getURL() const;
    [[nodiscard]] std::string getHotline() const;
    [[nodiscard]] std::string getSysUserId() const;
    [[nodiscard]] std::string getCreatedTime() const;
    [[nodiscard]] std::string getUpdatedTime() const;
    [[nodiscard]] short getCategoryId() const;

    void setEncryptedPwd(const std::vector<unsigned char>& newEncryptedPwd);
    void setIv(const std::vector<unsigned char>& newIv);
    void setPhoneId(short newPhoneId);
    void setEmailId(short newEmailId);

    void setId(int newId);
    void setUserId(const std::string& newUserId);
    void setAlias(const std::string& newAlias);
    void setSubAccount(const std::string& newSubAccount);
    void setPostscript(const std::string& newPostscript);
    void setPlatformName(const std::string& newPlatformName);
    void setProviderName(const std::string& newProviderName);
    void setURL(const std::string& newURL);
    void setHotline(const std::string& newHotline);
    void setSysUserId(const std::string& newSysUserId);
    void setCreatedTime(const std::string& newCreatedTime);
    void setUpdatedTime(const std::string& newUpdatedTime);
    void setCategoryId(short newCategoryId);
};

class ViewAccount final : public Account {
private:
    std::string phone;
    std::string email;
    std::string category;

public:
    ViewAccount();
    ViewAccount(int id, std::string userId, std::string alias, std::string subAccount, std::string phone, std::string email, std::string postscript, std::string platformName,
                std::string providerName, std::string URL, std::string hotline, std::string category, std::string sysUserId, std::string createdTime, std::string updatedTime);
    ViewAccount(ViewAccount const& viewAccount);
    [[nodiscard]] std::string getPhone() const;
    [[nodiscard]] std::string getEmail() const;
    [[nodiscard]] std::string getCategory() const;
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setCategory(const std::string& category);

    void copy(const ViewAccount& viewAccount);
};

#endif
