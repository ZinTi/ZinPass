#ifndef AUTH_SERVICE_H
#define AUTH_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "user/v1/auth.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{
class AuthServiceImpl final : public user::v1::AuthService::Service {
public:
    grpc::Status AuthUser(grpc::ServerContext* context, const user::v1::AuthUserRequest* request, user::v1::AuthUserResponse* response) override;
    grpc::Status CreateSession(ServerContext* context, const user::v1::CreateSessionRequest* request, user::v1::CreateSessionResponse* response) override;
    grpc::Status UpdateSession(ServerContext* context, const user::v1::UpdateSessionRequest* request, user::v1::UpdateSessionResponse* response) override;
    grpc::Status DeleteSession(ServerContext* context, const user::v1::DeleteSessionRequest* request, user::v1::DeleteSessionResponse* response) override;
};
}

#endif //AUTH_SERVICE_H
