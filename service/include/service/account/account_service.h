#ifndef ACCOUNT_SERVICE_H
#define ACCOUNT_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/account.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service {
class AccountServiceImpl final : public account::v1::AccountService::Service {
public:

    grpc::Status ListEmailAddresses(ServerContext* context, const account::v1::ListEmailAddressesRequest* request, account::v1::ListEmailAddressesResponse* response) override;

    grpc::Status CreateAccount(ServerContext *context, const account::v1::CreateAccountRequest *request, account::v1::CreateAccountResponse *response) override;

    grpc::Status ListAccountById(ServerContext* context, const account::v1::ListAccountByIdRequest* request, account::v1::ListAccountByIdResponse* response) override;

    grpc::Status ListAccounts(ServerContext* context, const account::v1::ListAccountsRequest* request, account::v1::ListAccountsResponse* response) override;

    grpc::Status UpdateAccount(ServerContext *context, const account::v1::UpdateAccountRequest *request, account::v1::UpdateAccountResponse *response) override;

    grpc::Status DeleteAccount(ServerContext *context, const account::v1::DeleteAccountRequest *request, account::v1::DeleteAccountResponse *response) override;

    grpc::Status FetchPassword(ServerContext* context, const account::v1::FetchPasswordRequest* request, account::v1::FetchPasswordResponse* response) override;

    grpc::Status ChangePassword(ServerContext* context, const account::v1::ChangePasswordRequest* request, account::v1::ChangePasswordResponse* response) override;

};

}
#endif //ACCOUNT_SERVICE_H
