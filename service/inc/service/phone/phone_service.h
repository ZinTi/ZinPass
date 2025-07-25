#ifndef PHONE_SERVICE_H
#define PHONE_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/phone.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service{
class PhoneServiceImpl final : public account::v1::PhoneService::Service {
public:
    grpc::Status CreatePhone(ServerContext *context, const account::v1::CreatePhoneRequest *request, account::v1::CreatePhoneResponse *response) override;

    grpc::Status ListPhoneNumbers(ServerContext* context, const account::v1::ListPhoneNumbersRequest* request, account::v1::ListPhoneNumbersResponse* response) override;

    grpc::Status ListPhone(ServerContext *context, const account::v1::ListPhoneRequest *request, account::v1::ListPhoneResponse *response) override;

    grpc::Status FindPhoneById(ServerContext* context, const account::v1::FindPhoneByIdRequest* request, account::v1::FindPhoneByIdResponse* response) override;

    grpc::Status UpdatePhone(ServerContext *context, const account::v1::UpdatePhoneRequest *request, account::v1::UpdatePhoneResponse *response) override;

    grpc::Status DeletePhone(ServerContext *context, const account::v1::DeletePhoneRequest *request, account::v1::DeletePhoneResponse *response) override;
};

}
#endif
