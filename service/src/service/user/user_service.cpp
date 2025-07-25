#include "service/user/user_service.h"
#include "business/user_manager.h"

namespace zinpass::service{

grpc::Status UserServiceImpl::CreateUser(ServerContext* context, const user::v1::CreateUserRequest* request, user::v1::CreateUserResponse* response) {
    //std::cout << "[RPC] CreateUser Request: " << request->DebugString() << std::endl;
    // 1. 反序列化请求
    const std::string& username = request->username();
    const std::string& password = request->password();
    const std::string& nickname = request->nickname();
    // 2. 注册
    if (const auto [user_id, message] = business::UserManager::registerSysUser(username, nickname, password);
        36 == user_id.length()) {  // UUID 长 36
        // std::cout << "[RPC] User registered successfully: " << user_id << std::endl;
        response->set_id(user_id);
        response->set_message(message);
    } else {
        // std::cout << "[RPC] User not registered" << std::endl;
        response->set_id("");
        response->set_message(message);
    }

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::ListUsers(ServerContext* context, const user::v1::ListUsersRequest* request, user::v1::ListUsersResponse* response){

    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::ListUsernames(ServerContext* context, const user::v1::ListUsernamesRequest* request, user::v1::ListUsernamesResponse* response){
    //std::cout << "[RPC] ListUsernames Request: " << request->DebugString() << std::endl;

    const auto [data, message] = business::UserManager::ListUsernames();
    for(const auto& username : data){
        response->add_usernames(username);
    }
    response->set_message(message);
    return grpc::Status::OK;
}

grpc::Status UserServiceImpl::UpdateUser(ServerContext* context, const user::v1::UpdateUserRequest* request, user::v1::UpdateUserResponse* response){

    return grpc::Status::OK;
}

}