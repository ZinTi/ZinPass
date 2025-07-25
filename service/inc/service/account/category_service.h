#ifndef CATEGORY_SERVICE_H
#define CATEGORY_SERVICE_H
#include <grpcpp/grpcpp.h>
#include "account/v1/category.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;

namespace zinpass::service {
class CategoryServiceImpl final : public account::v1::CategoryService::Service {
public:

    grpc::Status ListNameOfCategories(ServerContext* context, const account::v1::ListNameOfCategoriesRequest* request, account::v1::ListNameOfCategoriesResponse* response) override;
};

}

#endif
