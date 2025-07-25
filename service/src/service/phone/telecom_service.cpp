#include "service/phone/telecom_service.h"
#include "mod_session/session_mgr.h"
#include "business/telecom_operator_manager.h"

namespace zinpass::service {

    grpc::Status TelecomServiceImpl::ListTelecomOperator(grpc::ServerContext *context, const account::v1::ListTelecomOperatorRequest *request, account::v1::ListTelecomOperatorResponse *response) {
        // std::cout << "[RPC] ListTelecomOperator Request: " << request->DebugString() << std::endl;
        // 1. 检查 session_id 有效性
        auto& session_mgr = mod_session::SessionMgr::get_instance();
        const std::string user_id = session_mgr.validate_session_and_get_user_id(request->session_id());
        if (user_id.empty()){
            // std::cout << "[RPC] Invalid Session ID" << std::endl;
            response->set_message("无效会话");
            return grpc::Status::OK;
        }

        const auto[telecom_operator_options, message] = business::TelecomOperatorManager::get_telecom_operator_options();
        for(const auto& telecom : telecom_operator_options){
            response->add_telecoms(telecom);
        }
        response->set_message(message);
        return grpc::Status::OK;
    }
}