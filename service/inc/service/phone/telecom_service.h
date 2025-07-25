#ifndef TELECOM_SERVICE_H
#define TELECOM_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/telecom.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service {

class TelecomServiceImpl final : public account::v1::TelecomService::Service {
public:
    grpc::Status ListTelecomOperator(ServerContext *context, const account::v1::ListTelecomOperatorRequest *request, account::v1::ListTelecomOperatorResponse *response) override;
};

}

#endif

