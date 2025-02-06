#ifndef TELECOMOPERATORSERVICE_H
#define TELECOMOPERATORSERVICE_H

#include <string>
#include <vector>

class TelecomOperatorService {
public:
    /**
     * @brief getTelecomOperatorOptions
     * @param msg 反馈信息
     * @param telecomOperator 获取到的邮箱地址
     * @return 成功与否
     */
    bool getTelecomOperatorOptions(std::string& msg, std::vector<std::string>& telecomOperator);
};

#endif
