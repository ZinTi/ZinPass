#include "user_rpc.h"
#include <QDebug>

namespace zinpass::rpc {

    UserRPC::UserRPC(std::shared_ptr<grpc::Channel> channel)
        : stub_(user::v1::UserService::UserService::NewStub(channel)) {}

    UserRPCReturn<std::optional<std::string>> UserRPC::register_account(const std::string& username, const std::string& password, const std::string& nickname){
        // 1. Construct request
        user::v1::CreateUserRequest request;
        request.set_username(username);
        request.set_password(password);
        request.set_nickname(nickname);
        // 2. Construct response
        user::v1::CreateUserResponse response;
        // 3. Create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->CreateUser(&context, request, &response);
        if(!status.ok()){
            // qDebug() << "RPC failed: " << status.error_message() << "\n";
            return {std::nullopt, "RPC failed"};
        }
        return {response.id(), response.message()};
    }

    int UserRPC::list_usernames(std::vector<std::string>& usernames) {
        // 1. construct request
        user::v1::ListUsernamesRequest request;
        request.set_message("list usernames");
        // 2. construct response
        user::v1::ListUsernamesResponse response;
        // 3. create client context
        grpc::ClientContext context;
        // 4. Initiate RPC call and fetch status
        grpc::Status status = stub_->ListUsernames(&context, request, &response);
        if (!status.ok()) {
            return -1;
        }
        for (const auto& username : response.usernames()) {
            usernames.push_back(username);
        }
        return 0;
    }

}
