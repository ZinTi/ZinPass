#include "account/account_service.h"

#include "account_manager.h"
#include "authentication.h"
#include "session_manager.h"
#include "pwd_utils.h"

namespace zinpass::service {

grpc::Status AccountServiceImpl::ListEmailAddresses(ServerContext* context, const account::v1::ListEmailAddressesRequest* request, account::v1::ListEmailAddressesResponse* response) {
    // std::cout << "[RPC] ListEmailAddresses Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    const auto[email_addresses, message] = business::AccountManager::get_email_options(user_id);
    for (const auto& email_address : email_addresses){
        response->add_email_addresses(email_address);
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status AccountServiceImpl::ListAccountById(ServerContext* context, const account::v1::ListAccountByIdRequest* request, account::v1::ListAccountByIdResponse* response) {
    // std::cout << "[RPC] ListAccountById Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 解析请求体
    const long long account_id = request->account_id();
    const auto[data, message] = business::AccountManager::get_account_by_id(account_id);
    response->set_message(message);
    auto* v_acc = new account::v1::ViewAccount();
    v_acc->set_account_id(data.getId());
    v_acc->set_provider_name(data.getProviderName());
    v_acc->set_platform_name(data.getPlatformName());
    v_acc->set_username(data.getUsername());
    v_acc->set_nickname(data.getNickname());
    v_acc->set_sub_account(data.getSubAccount());
    v_acc->set_mobile_phone(data.getPhone());
    v_acc->set_email(data.getEmail());
    v_acc->set_postscript(data.getPostscript());
    v_acc->set_url(data.getURL());
    v_acc->set_hotline(data.getHotline());
    v_acc->set_category(data.getCategory());
    v_acc->set_created_time(data.getCreatedTime());
    v_acc->set_updated_time(data.getUpdatedTime());
    response->set_allocated_account(v_acc);
    return grpc::Status::OK;
}

grpc::Status AccountServiceImpl::CreateAccount(ServerContext* context, const account::v1::CreateAccountRequest* request, account::v1::CreateAccountResponse* response){
    // std::cout << "[RPC] CreateAccount Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 验证主密码是否正确，若错误则结束
    const auto[ret_validate, message_validate] = business::Authentication::validateUserById(user_id, request->main_password());
    if (false == ret_validate) {
        response->set_result(false);
        response->set_message(message_validate);
        return grpc::Status::OK;
    }

    // 3. 新增
    const std::string& main_password = request->main_password();
    models::ViewAccount view_account(
        0,
        request->account().username(),
        request->account().nickname(),
        request->account().sub_account(),
        request->account().postscript(),
        request->account().platform_name(),
        request->account().provider_name(),
        request->account().url(),
        request->account().hotline(),
        user_id,
        "",
        "",
        request->account().mobile_phone(),
        request->account().email(),
        request->account().category()
    );
    const auto[ret_add, ret_message] = business::AccountManager::add_view_account(main_password, view_account, request->password());
    response->set_result(ret_add);
    response->set_message(ret_message);
    return grpc::Status::OK;
}


/*
grpc::Status AccountServiceImpl::ListAccounts(ServerContext* context, const account::v1::ListAccountsRequest* request, account::v1::ListAccountsResponse* response) {
    // std::cout << "[RPC] ListAccounts Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 解析请求体
    const int32_t page_size = request->page_size();
    if (request->has_filter()) {
        const common::v1::Filter& filter = request->filter();

        // 提取逻辑操作符 (AND/OR)
        const common::v1::LogicalOperator op = filter.operator_();
        if (op == common::v1::LogicalOperator::AND) {
            std::cout << "Logical operator: AND" << std::endl;
        } else if (op == common::v1::LogicalOperator::OR) {
            std::cout << "Logical operator: OR" << std::endl;
        }

        // 遍历所有条件
        for (int i = 0; i < filter.conditions_size(); ++i) {
            const common::v1::Condition& condition = filter.conditions(i);
            const std::string& field = condition.field();
            const common::v1::Operator condition_op = condition.op();
            std::cout << "Condition " << i << ": "
            << "Field=" << field
            << ", Op=" << Operator_Name(condition_op)  // 枚举转字符串
            << std::endl;

            const google::protobuf::Value& value = condition.value();
            switch (value.kind_case()) {
                case google::protobuf::Value::kNumberValue: {
                double num = value.number_value();
                std::cout << "  Value (number): " << num << std::endl;
                break;
            }
            case google::protobuf::Value::kStringValue: {
                const std::string& str = value.string_value();
                std::cout << "  Value (string): " << str << std::endl;
                break;
            }
            case google::protobuf::Value::kBoolValue: {
                bool b = value.bool_value();
                std::cout << "  Value (bool): " << b << std::endl;
                break;
            }
            default:
                std::cerr << "Unsupported value type" << std::endl;
            }
        }
    }

    std::string message;
    std::vector<models::ViewAccount> accounts;
    std::string provider_name_f;
    std::string platform_name_f;
    std::string username_f;
    std::string nickname_f;
    std::string telephone_f;
    std::string email_f;
    const auto ret = business::AccountManager::getAccounts(
        message,
        provider_name_f,
        platform_name_f,
        username_f,
        nickname_f,
        telephone_f,
        email_f,
        user_id,
        accounts
        );

    return grpc::Status::OK;
}
*/
grpc::Status AccountServiceImpl::ListAccounts(ServerContext* context, const account::v1::ListAccountsRequest* request, account::v1::ListAccountsResponse* response) {
    // std::cout << "[RPC] ListAccounts Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 解析请求体
    const account::v1::Account& conditions = request->conditions();
    const int32_t page_size = request->page_size();

    const auto[v_accounts, message] = business::AccountManager::get_accounts(
        conditions.provider_name(),
        conditions.platform_name(),
        conditions.username(),
        conditions.nickname(),
        conditions.mobile_phone(),
        conditions.email(),
        user_id
        );

    // std::cout << "[RPC_DEBUG] accounts total: " << v_accounts.size() << std::endl;
    response->set_message(message);
    for (const auto& data : v_accounts) {
        account::v1::ViewAccount* v_acc = response->add_accounts();
        v_acc->set_account_id(data.getId());
        v_acc->set_provider_name(data.getProviderName());
        v_acc->set_platform_name(data.getPlatformName());
        v_acc->set_username(data.getUsername());
        v_acc->set_nickname(data.getNickname());
        v_acc->set_sub_account(data.getSubAccount());
        v_acc->set_mobile_phone(data.getPhone());
        v_acc->set_email(data.getEmail());
        v_acc->set_postscript(data.getPostscript());
        v_acc->set_url(data.getURL());
        v_acc->set_hotline(data.getHotline());
        v_acc->set_category(data.getCategory());
        v_acc->set_created_time(data.getCreatedTime());
        v_acc->set_updated_time(data.getUpdatedTime());
    }
    return grpc::Status::OK;
}

grpc::Status AccountServiceImpl::UpdateAccount(ServerContext *context, const account::v1::UpdateAccountRequest *request, account::v1::UpdateAccountResponse *response) {
    // std::cout << "[RPC] UpdateAccount Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()) {
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 检查是否需要更新密码
    const std::string& account_password = request->account_password();
    if (false == account_password.empty()) {    // 需要更新账号额密码和账号的基本信息
        // 检查主密码（密钥）是否正确
        const auto[ret_validate, msg_validate] = business::Authentication::validateUserById(user_id, request->main_password());
        if (false == ret_validate) {
            response->set_result(false);
            response->set_message(msg_validate);
            return grpc::Status::OK;    // 主密码兼密钥不正确，中止更新
        }
        const auto[ret_change_pwd, msg_change_pwd] = business::AccountManager::update_account_password(
            request->account_id(),user_id,request->main_password(),request->account_password());
        if (false == ret_change_pwd) {
            response->set_result(false);
            response->set_message(msg_change_pwd);
            return grpc::Status::OK;    // 更新账号的密码失败，中止更新基本信息
        }
    }

    // 3. 更新账号基本信息
    const models::ViewAccount view_account(
        request->account_id(),
        request->account().username(),
        request->account().nickname(),
        request->account().sub_account(),
        request->account().postscript(),
        request->account().platform_name(),
        request->account().provider_name(),
        request->account().url(),
        request->account().hotline(),
        user_id,
        "",
        "",
        request->account().mobile_phone(),
        request->account().email(),
        request->account().category()
        );
    const auto[success, message_update] = business::AccountManager::update_account_base_info(view_account);
    response->set_result(success);
    response->set_message(message_update);
    return grpc::Status::OK;
}

grpc::Status AccountServiceImpl::DeleteAccount(ServerContext *context, const account::v1::DeleteAccountRequest *request, account::v1::DeleteAccountResponse *response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 检查密码是否正确
    const auto[ret_validate, message_validate] = business::Authentication::validateUserById(user_id, request->main_password());
    if(false == ret_validate){
        response->set_message(message_validate);
        response->set_result(false);
        return grpc::Status::OK;
    }

    // 3. 删除操作
    const long long account_id = request->account_id();
    const auto[ret_delete, message_delete] = business::AccountManager::delete_account(account_id);
    response->set_result(ret_delete);
    response->set_message(message_delete);
    return grpc::Status::OK;
}


grpc::Status AccountServiceImpl::FetchPassword(ServerContext* context, const account::v1::FetchPasswordRequest* request, account::v1::FetchPasswordResponse* response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 验证主密码是否正确
    const auto[result, message_validate] = business::Authentication::validateUserById(user_id, request->main_password());
    if (false == result) {
        response->set_password("");
        response->set_message(message_validate);
        return grpc::Status::OK;
    }

    // 3. 若密码正确查询密码
    const long long account_id = request->account_id();
    const std::string& main_password = request->main_password();
    const auto[plaintext_password, message_plaintext] = business::AccountManager::retrieve_decrypted_password(account_id, user_id, main_password);
    if (plaintext_password.empty()) {
        response->set_message(message_plaintext);
        return grpc::Status::OK;
    }
    response->set_password(plaintext_password);
    response->set_message(message_plaintext);
    return grpc::Status::OK;
}

grpc::Status AccountServiceImpl::ChangePassword(ServerContext* context, const account::v1::ChangePasswordRequest* request, account::v1::ChangePasswordResponse* response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    return grpc::Status::OK;
}


}
