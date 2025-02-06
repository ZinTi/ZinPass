#include "Account.h"
#include <utility>

Account::Account() {
    id = 0;
    userId = "";
    alias = "";
    subAccount = "";
    postscript = "";
    platformName = "";
    providerName = "";
    URL = "";
    hotline = "";
    sysUserId = "";
    createdTime = "";
    updatedTime = "";

    encryptedPwd;
    iv;
    phoneId = 0;
    emailId = 0;
    categoryId = 0;
}

Account::Account(std::vector<unsigned char> encryptedPwd, std::vector<unsigned char> iv, short phoneId, short emailId,
            int id, std::string userId, std::string alias, std::string subAccount, std::string postscript, std::string platformName, std::string providerName,
            std::string URL, std::string hotline, short categoryId, std::string sysUserId, std::string createdTime, std::string updatedTime)
    : id(id), userId(std::move(userId)), alias(std::move(alias)), subAccount(std::move(subAccount)), postscript(std::move(postscript)), platformName(std::move(platformName)), providerName(std::move(providerName)), URL(std::move(URL)),
      hotline(std::move(hotline)), sysUserId(std::move(sysUserId)), createdTime(std::move(createdTime)),updatedTime(std::move(updatedTime)), encryptedPwd(std::move(encryptedPwd)), phoneId(phoneId),emailId(emailId), categoryId(categoryId) {
}

Account::Account(Account const& account) {
    encryptedPwd = account.encryptedPwd;
    iv = account.iv;
    phoneId = account.phoneId;
    emailId = account.emailId;

    id = account.id;
    userId = account.userId;
    alias = account.alias;
    subAccount = account.subAccount;
    postscript = account.postscript;
    platformName = account.platformName;
    providerName = account.providerName;
    URL = account.URL;
    hotline = account.hotline;
    categoryId = account.categoryId;
    sysUserId = account.sysUserId;
    createdTime = account.createdTime;
    updatedTime = account.updatedTime;
}

// Account的getter方法

const std::vector<unsigned char>& Account::getEncryptedPwd() const{
    return encryptedPwd;
}
const std::vector<unsigned char>& Account::getIv() const{
    return iv;
}
short Account::getPhoneId() const {
    return phoneId;
}
short Account::getEmailId() const {
    return emailId;
}

int Account::getId() const {
    return id;
}

std::string Account::getUserId() const {
    return userId;
}

std::string Account::getAlias() const {
    return alias;
}

std::string Account::getSubAccount() const {
    return subAccount;
}

std::string Account::getPlatformName() const {
    return platformName;
}

std::string Account::getProviderName() const {
    return providerName;
}

std::string Account::getURL() const {
    return URL;
}

std::string Account::getHotline() const {
    return hotline;
}

std::string Account::getSysUserId() const {
    return sysUserId;
}

std::string Account::getPostscript() const {
    return postscript;
}

std::string Account::getCreatedTime() const {
    return createdTime;
}

std::string Account::getUpdatedTime() const {
    return updatedTime;
}

short Account::getCategoryId() const {
    return categoryId;
}

// Account的setter方法
void Account::setEncryptedPwd(const std::vector<unsigned char>& newEncryptedPwd){
  this->encryptedPwd = newEncryptedPwd;
}
void Account::setIv(const std::vector<unsigned char>& newIv){
  this->iv = newIv;
}
void Account::setPhoneId(const short newPhoneId) {
    this->phoneId = newPhoneId;
}
void Account::setEmailId(const short newEmailId) {
    this->emailId = newEmailId;
}

void Account::setId(const int newId) {
    this->id = newId;
}

void Account::setUserId(const std::string& newUserId) {
    this->userId = newUserId;
}

void Account::setAlias(const std::string& newAlias) {
    this->alias = newAlias;
}

void Account::setSubAccount(const std::string& newSubAccount) {
    this->subAccount = newSubAccount;
}

void Account::setPostscript(const std::string& newPostscript) {
    this->postscript = newPostscript;
}

void Account::setPlatformName(const std::string& newPlatformName) {
    this->platformName = newPlatformName;
}

void Account::setProviderName(const std::string& newProviderName) {
    this->providerName = newProviderName;
}

void Account::setURL(const std::string& newURL) {
    this->URL = newURL;
}

void Account::setHotline(const std::string& newHotline) {
    this->hotline = newHotline;
}

void Account::setSysUserId(const std::string& newSysUserId) {
    this->sysUserId = newSysUserId;
}

void Account::setCreatedTime(const std::string& newCreatedTime) {
    this->createdTime = newCreatedTime;
}

void Account::setUpdatedTime(const std::string& newUpdatedTime) {
    this->updatedTime = newUpdatedTime;
}

void Account::setCategoryId(const short newCategoryId) {
    this->categoryId = newCategoryId;
}

// ViewAccount
ViewAccount::ViewAccount() {
    id = 0;
    userId = "";
    alias = "";
    // password = "";
    subAccount = "";
    // phoneId = 0;
    // emailId = 0;
    postscript = "";
    platformName = "";
    providerName = "";
    URL = "";
    hotline = "";
    // categoryId = 0;
    sysUserId = "";
    createdTime = "";
    updatedTime = "";
    phone = "";
    email = "";
    category = "";
}

ViewAccount::ViewAccount(const int id, std::string userId, std::string alias, std::string subAccount, std::string phone, std::string email, std::string postscript, std::string platformName,
                         std::string providerName, std::string URL, std::string hotline, std::string category, std::string sysUserId, std::string createdTime, std::string updatedTime) {
    this->id = id;
    this->userId = std::move(userId);
    this->alias = std::move(alias);
    this->subAccount = std::move(subAccount);

    this->phone = std::move(phone);
    this->email = std::move(email);

    this->postscript = std::move(postscript);
    this->platformName = std::move(platformName);
    this->providerName = std::move(providerName);
    this->URL = std::move(URL);
    this->hotline = std::move(hotline);
    this->category = std::move(category);
    this->sysUserId = std::move(sysUserId);
    this->createdTime = std::move(createdTime);
    this->updatedTime = std::move(updatedTime);
}

ViewAccount::ViewAccount(ViewAccount const& viewAccount) {
    id = viewAccount.id;
    userId = viewAccount.userId;
    alias = viewAccount.alias;
    subAccount = viewAccount.subAccount;
    phone = viewAccount.phone;
    email = viewAccount.email;
    postscript = viewAccount.postscript;
    platformName = viewAccount.platformName;
    providerName = viewAccount.providerName;
    URL = viewAccount.URL;
    hotline = viewAccount.hotline;
    category = viewAccount.category;
    sysUserId = viewAccount.sysUserId;
    createdTime = viewAccount.createdTime;
    updatedTime = viewAccount.updatedTime;
}

std::string ViewAccount::getPhone() const {
    return phone;
}

std::string ViewAccount::getEmail() const {
    return email;
}

std::string ViewAccount::getCategory() const {
    return category;
}

void ViewAccount::setPhone(const std::string& newPhone) {
    this->phone = newPhone;
}

void ViewAccount::setEmail(const std::string& newEmail) {
    this->email = newEmail;
}

void ViewAccount::setCategory(const std::string& newCategory) {
    this->category = newCategory;
}

void ViewAccount::copy(const ViewAccount& viewAccount) {
    id = viewAccount.id;
    userId = viewAccount.userId;
    alias = viewAccount.alias;
    subAccount = viewAccount.subAccount;
    phone = viewAccount.phone;
    email = viewAccount.email;
    postscript = viewAccount.postscript;
    platformName = viewAccount.platformName;
    providerName = viewAccount.providerName;
    URL = viewAccount.URL;
    hotline = viewAccount.hotline;
    category = viewAccount.category;
    sysUserId = viewAccount.sysUserId;
    createdTime = viewAccount.createdTime;
    updatedTime = viewAccount.updatedTime;
}
