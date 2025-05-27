#include "phone/phone_service.h"
#include "session_manager.h"
#include "mobile_phone_manager.h"

namespace zinpass::service{

grpc::Status PhoneServiceImpl::CreatePhone(ServerContext *context, const account::v1::CreatePhoneRequest *request, account::v1::CreatePhoneResponse *response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 新增手机号
    const auto[result, message] = business::MobilePhoneManager::add_mobile_phone(
        user_id,
        request->phone().phone_number(),
        request->phone().telecom_operator(),
        request->phone().service_pwd(),
        request->phone().pin(),
        request->phone().puk(),
        request->phone().join_time(),
        request->phone().phone_area(),
        request->phone().postscript()
        );
    response->set_result(result);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::ListPhoneNumbers(ServerContext* context, const account::v1::ListPhoneNumbersRequest* request, account::v1::ListPhoneNumbersResponse* response){
    // std::cout << "[RPC] ListPhoneNumbers Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    const auto[phone_numbers, message] = business::MobilePhoneManager::get_phone_options(user_id);
    for(const auto& phone_number : phone_numbers){
        response->add_phone_numbers(phone_number);
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::ListPhone(ServerContext *context, const account::v1::ListPhoneRequest *request, account::v1::ListPhoneResponse *response) {
    // std::cout << "[RPC] ListPhone Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    const std::string& req_telecom_operator = request->telecom();
    const std::string& req_phone_number = request->phone_number();
    const auto[phone_numbers, message] = business::MobilePhoneManager::get_mobile_phones(
        user_id,
        req_telecom_operator,
        req_phone_number
    );
    for (const auto& phone_number : phone_numbers) {
        account::v1::Phone* phone = response->add_phone_numbers();
        phone->set_phone_id(phone_number.getId());
        phone->set_phone_number(phone_number.getPhoneNumber());
        phone->set_telecom_operator(phone_number.getTelecomOperator());
        phone->set_service_pwd(phone_number.getServicePwd());
        phone->set_pin(phone_number.getPin());
        phone->set_puk(phone_number.getPuk());
        phone->set_join_time(phone_number.getJoinTime());
        phone->set_phone_area(phone_number.getPhoneArea());
        phone->set_postscript(phone_number.getPostscript());
        phone->set_sys_user_id(phone_number.getSysUserId());
        phone->set_created_time(phone_number.getCreatedTime());
        phone->set_updated_time(phone_number.getUpdatedTime());
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::FindPhoneById(ServerContext* context, const account::v1::FindPhoneByIdRequest* request, account::v1::FindPhoneByIdResponse* response) {
    // std::cout << "[RPC] FindPhoneById Request: " << request->DebugString() << std::endl;
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }
    const auto[data, message] = business::MobilePhoneManager::get_phone_by_owner_and_id(
        user_id,
        request->phone_id()
    );

    account::v1::Phone* phone = new account::v1::Phone();
    phone->set_phone_id(data.getId());
    phone->set_phone_number(data.getPhoneNumber());
    phone->set_telecom_operator(data.getTelecomOperator());
    phone->set_service_pwd(data.getServicePwd());
    phone->set_pin(data.getPin());
    phone->set_puk(data.getPuk());
    phone->set_join_time(data.getJoinTime());
    phone->set_phone_area(data.getPhoneArea());
    phone->set_postscript(data.getPostscript());
    phone->set_sys_user_id(data.getSysUserId());
    phone->set_created_time(data.getCreatedTime());
    phone->set_updated_time(data.getUpdatedTime());
    response->set_allocated_phone_number(phone);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::UpdatePhone(ServerContext *context, const account::v1::UpdatePhoneRequest *request, account::v1::UpdatePhoneResponse *response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 更新 Phone
    const auto[result, message] = business::MobilePhoneManager::update_mobile_phone(
        request->phone_id(),
        user_id,
        request->phone().phone_number(),
        request->phone().telecom_operator(),
        request->phone().service_pwd(),
        request->phone().pin(),
        request->phone().puk(),
        request->phone().join_time(),
        request->phone().phone_area(),
        request->phone().postscript()
        );
    response->set_result(result);
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status PhoneServiceImpl::DeletePhone(ServerContext *context, const account::v1::DeletePhoneRequest *request, account::v1::DeletePhoneResponse *response) {
    // 1. 检查 session_id 有效性
    const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
    if (user_id.empty()){
        // std::cout << "[RPC] Invalid Session ID" << std::endl;
        response->set_message("无效会话");
        return grpc::Status::OK;
    }

    // 2. 删除 Phone
    const auto[result, message] = business::MobilePhoneManager::delete_mobile_phone(request->phone_id(), user_id);
    response->set_result(result);
    response->set_message(message);
    return grpc::Status::OK;
}

}