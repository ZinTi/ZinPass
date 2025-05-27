#include "user/auth_service.h"
#include "authentication.h"

namespace zinpass::service{

grpc::Status AuthServiceImpl::AuthUser(grpc::ServerContext* context, const user::v1::AuthUserRequest* request, user::v1::AuthUserResponse* response) {
    //std::cout << "[RPC] AuthUser Request: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::CreateSession(ServerContext* context, const user::v1::CreateSessionRequest* request, user::v1::CreateSessionResponse* response) {
    //std::cout << "[RPC] CreateSession Request: " << request->DebugString() << std::endl;
    // 1. 反序列化请求
    const std::string& username = request->username();
    const std::string& password = request->password();
    // const std::string& captcha_input = request->captcha_input();
    // 2. 建立会话
    if(const auto [data, message] = business::Authentication::loginByUsername(username, password);
        std::nullopt == data) {
        // std::cout << "[RPC] Session creation failed" << std::endl;
        response->set_message(message);
    } else {
        // std::cout << "[RPC] Session created successfully, session_id: " << data->session_id << std::endl;
        response->set_session_id(data->session_id);
        response->set_user_id(data->user.getId());
        response->set_username(data->user.getUsername());
        response->set_nickname(data->user.getNickname());
        response->set_message(message);
    }

    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::UpdateSession(ServerContext* context, const user::v1::UpdateSessionRequest* request, user::v1::UpdateSessionResponse* response) {
    //std::cout << "[RPC] UpdateSession Request: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

grpc::Status AuthServiceImpl::DeleteSession(ServerContext* context, const user::v1::DeleteSessionRequest* request, user::v1::DeleteSessionResponse* response) {
    //std::cout << "[RPC] DeleteSession Request: " << request->DebugString() << std::endl;
    return grpc::Status::OK;
}

}