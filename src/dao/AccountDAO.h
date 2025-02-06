#ifndef ACCOUNTDAO_H
#define ACCOUNTDAO_H

#include "Account.h"
#include "sqlite3.h"
#include <map>
#include <string>
#include <vector>
#include <variant>

class AccountDAO {
public:
	// 定义在类中的枚举类
	enum class FilterType {
		ALL, // 不筛选（包括NULL、""在内的任意值）
		LIKE, // 模糊匹配
		EXACT, // 精确查询
		IS_NULL // 筛选只为NULL的情况
	};

	enum class ColumnType {
		id,
		userId,
		alias,
		password,
		subAccount,
		phoneId,
		emailId,
		postscript,
		platformName,
		providerName,
		URL,
		hotline,
		categoryId,
		owner,
		createdTime,
		updatedTime
	};

	// 定义存储不同类型值的结构体
	struct KeyValuePair {
		ColumnType key;
		std::variant<int, char, std::string, void*, bool> value;
	};

public:
	explicit AccountDAO(const std::string& dbPath);
	~AccountDAO();

	/**
	 * @brief 根据id查询目标viewAccount记录
	 * @param msg 反馈信息
	 * @param id id
	 * @param viewAccount 查询到的viewAccount信息
	 * @return 成功与否
	 */
	bool findById(std::string& msg, int id, ViewAccount& viewAccount);

	/**
	 * @brief 查询视图view_account以获取所有指定sys_user_id的记录
	 * @param msg 反馈信息
	 * @param sysUserId 筛选条件：当前登录用户的ID
	 * @param viewAccounts 保存查询到的viewAccount信息的vector容器
	 * @return 成功与否
	 */
	[[deprecated("不推荐使用，使用另一个函数代替")]]
	bool find(std::string& msg, const std::string& sysUserId, std::vector<ViewAccount>& viewAccounts);

	/**
	 * @brief 查询所有符合条件的记录，使用模糊查询
	 * @param msg 反馈信息
	 * @param providerName 服务商名
	 * @param platformName 平台名
	 * @param userId 账号
	 * @param alias 昵称
	 * @param phone 绑定手机号
	 * @param email 绑定邮箱
	 * @param updateTimeRangeStart 更新时间范围左区间
	 * @param updateTimeRangeEnd 更新时间范围右区间
	 * @param sysUserId 指定系统用户
	 * @param viewAccounts 保存查询到的viewAccount信息的vector容器
	 * @return 成功与否
	 */
	bool find(std::string& msg, const std::string& providerName, const std::string& platformName,
	          const std::string& userId, const std::string& alias, const std::string& phone, const std::string& email,
	          const std::string& updateTimeRangeStart, const std::string& updateTimeRangeEnd,
	          const std::string& sysUserId, std::vector<ViewAccount>& viewAccounts);

    /**
     * @brief 根据id查询目标account的密码
     * @param msg 反馈信息
     * @param id 查询目标的id
     * @param encryptedPwd 获取到的密码
     * @param iv 查询到的初始化向量值
     * @param sysUserId 获取到的密码记录的所属用户
     * @return 成功与否
     */
    bool findEncryptedPwdAndIvById(std::string& msg, int id, std::vector<unsigned char>& encryptedPwd, std::vector<unsigned char>& iv, std::string& sysUserId);

	/**
	 * @brief 新增Account记录
	 * @param msg 反馈信息
     * @param account 新Account记录
     * account.id不需要设置，会自动生成,
     * account.emailId < 0 时，设置为NULL,
     * account.phoneId < 0 时, 设置为NULL
	 * @return 成功与否
	 */
	bool add(std::string& msg, const Account& account);

	bool update(std::string& msg, const std::vector<KeyValuePair>& data, int id);


	/**
	 * @brief 修改Account表中的产品信息（1/4）
	 * @param msg 反馈信息
	 * @param id 筛选条件
	 * @param providerName 服务商名
	 * @param platformName 平台名
	 * @param url 平台网址
	 * @param hotline 服务热线
	 * @param updateTime 更新时间
	 * @return 成功与否
	 */
	bool update(std::string& msg, int id, const std::string& providerName, const std::string& platformName,
	            const std::string& url, const std::string& hotline, const std::string& updateTime);

    /**
     * @brief 修改Account表中的账号信息（2/4）
     * @param msg 反馈信息
     * @param id 筛选条件
     * @param userId 账号
     * @param alias 昵称
     * @param encryptedPwd 密码
     * @param iv 初始化向量新值
     * @param categoryId 类别，小于 0 时 设置为 NULL
     * @param updateTime 更新时间
     * @return 成功与否
     */
    bool update(std::string& msg, int id, const std::string& userId, const std::string& alias, const std::vector<unsigned char>& encryptedPwd,
                const std::vector<unsigned char>& iv, short categoryId, const std::string& updateTime);

	/**
	 * @brief 修改Account表中的第三方绑定账号信息（3/4）
	 * @param msg 反馈信息
	 * @param id 筛选条件
	 * @param phoneId 绑定手机号，小于 0 时 设置为 NULL
	 * @param emailId 绑定邮箱，小于 0 时 设置为 NULL
	 * @param updateTime 更新时间
	 * @return 成功与否
	 */
	bool update(std::string& msg, int id, int phoneId, int emailId, const std::string& updateTime);

	/**
	 * @brief 修改Account表中的其他信息（4/4）
	 * @param msg 反馈信息
	 * @param id 筛选条件
	 * @param subAccount 子账号
	 * @param postscript 备注（附言）
	 * @param updateTime 更新信息
	 * @return 成功与否
	 */
	bool update(std::string& msg, int id, const std::string& subAccount, const std::string& postscript,
	            const std::string& updateTime);

	/**
	 * @brief 删除Account记录
	 * @param msg 反馈信息
	 * @param id 删除目标的id
	 * @return 成功与否
	 */
	bool remove(std::string& msg, int id);

	// Email部分（特殊的Account）
	/**
	 * @brief 查询所有emailAddress
	 * @param msg 反馈信息
	 * @param sysUserId 系统用户账号
	 * @param emailAddrList 存放所有查询到的emailAddress的容器
	 * @return 成功与否
	 */
	bool findEmailList(std::string& msg, const std::string& sysUserId, std::vector<std::string>& emailAddrList);

	/**
	 * @brief emailAddressToId 将emailAddress转换成对应的id主键
	 * @param msg 反馈信息
	 * @param emailAddr 邮箱地址
	 * @param id 查询到的id主键，如果未查询到则 id 为 -1
	 * @return 成功与否
	 */
	bool emailAddressToId(std::string& msg, const std::string& emailAddr, short* id);

	/**
	 * @brief 根据id查询目标email记录
	 * @param msg 反馈信息
	 * @param id id
	 * @param email 保存查询到的email信息
	 * @return 成功与否
	 */
	bool findEmailById(std::string& msg, int id, ViewAccount& email);

	/**
	 * @brief 查询所有email记录
	 * @param msg 反馈信息
	 * @param emails 保存查询到的email信息的vector容器
	 * @param sysUserId 筛选条件：当前登录用户的ID
	 * @return 成功与否
	 */
	bool findAllEmail(std::string& msg, const std::string& sysUserId, std::vector<ViewAccount>& emails);

private:
	std::string dbPath; // 数据库路径
	sqlite3* db;
	void openDatabase(std::string& msg); // 打开数据库
	void closeDatabase() const; // 关闭数据库
	static void stmtToViewAccount(sqlite3_stmt* stmt, ViewAccount& viewAccount);
	static std::string buildSqlPart(const std::map<ColumnType, std::string>& data, const std::string& separator);

	/**
	 * @brief 生成主键
	 * @param msg 反馈信息
	 * @return 生成的id
	 */
	int generateId(std::string& msg);
};

#endif
