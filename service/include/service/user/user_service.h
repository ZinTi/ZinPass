#ifndef USER_SERVICE_H
#define USER_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "user/v1/user.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{

class UserServiceImpl final : public user::v1::UserService::Service {
public:
    grpc::Status CreateUser(ServerContext* context, const user::v1::CreateUserRequest* request, user::v1::CreateUserResponse* response) override;
    grpc::Status ListUsers(ServerContext* context, const user::v1::ListUsersRequest* request, user::v1::ListUsersResponse* response) override;
    grpc::Status ListUsernames(ServerContext* context, const user::v1::ListUsernamesRequest* request, user::v1::ListUsernamesResponse* response) override;
    grpc::Status UpdateUser(ServerContext* context, const user::v1::UpdateUserRequest* request, user::v1::UpdateUserResponse* response) override;
    // Status DeleteUser(user::v1::DeleteUserRequest* request);

};

}

#endif //USER_SERVICE_H
