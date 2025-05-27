#include "account/category_service.h"
#include "session_manager.h"
#include "account_manager.h"

namespace zinpass::service {

    grpc::Status CategoryServiceImpl::ListNameOfCategories(ServerContext* context, const account::v1::ListNameOfCategoriesRequest* request, account::v1::ListNameOfCategoriesResponse* response) {
        // std::cout << "[RPC] ListNameOfCategories request" << std::endl;
        // 1. 检查 session_id 有效性
        const std::string user_id = business::SessionManager::validate_session_and_get_user_id(request->session_id());
        if (user_id.empty()){
            // std::cout << "[RPC] Invalid Session ID" << std::endl;
            response->set_message("无效会话");
            return grpc::Status::OK;
        }

        const auto[categories, message] = business::AccountManager::get_category_options();
        for(const auto& category : categories){
            response->add_categories(category);
        }
        response->set_message(message);
        return grpc::Status::OK;
    }

}