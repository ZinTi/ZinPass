#include "telecom_operator_manager.h"
#include "telecom_operator_dao.h"

namespace zinpass::business{

TelecomOperatorManager::Return<std::vector<std::string>> TelecomOperatorManager::get_telecom_operator_options() {
    const repository::TelecomOperatorDAO telecom_operator_dao;
    std::vector<std::string> telecom_operator_options;
    std::string message;
    if (
        const repository::DaoStatus ret_status = telecom_operator_dao.findList(telecom_operator_options);
        repository::DaoStatus::Success != ret_status) {
        message = "发生错误";
    }else {
        message = "成功";
    }
    return {telecom_operator_options, message};
}

}