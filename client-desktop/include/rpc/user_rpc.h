#ifndef USERRPC_H
#define USERRPC_H

#include <string>
#include <optional>
#include <grpcpp/grpcpp.h>
#include "user.grpc.pb.h"

namespace zinpass::rpc{
    template<typename T>
    struct UserRPCReturn{
        T data;
        std::string message;
    };

    class UserRPC {
    public:
        // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
        UserRPC(std::shared_ptr<grpc::Channel> channel);

        // 2、调用远程服务的公共方法
        /**
         * @brief register_account
         * @param username 用户名
         * @param password 密码
         * @param nickname 昵称
         * @return （ user_id | std::nullopt ) + message
         */
        UserRPCReturn<std::optional<std::string>> register_account(const std::string& username, const std::string& password, const std::string& nickname);

        // int list_users(const std::string& session_id, std::vector<zinpass::models::User> users); // 获取用户列表（带分页）

        /**
         * @brief list_usernames
         * @param usernames 用户名
         * @return 0 表示完成
         */
        int list_usernames(std::vector<std::string>& usernames);

        /**
         * @brief update_user
         * @param session_id
         * @param username
         * @param nickname
         * @return ( bool | std::nullopt ) + message
         */
        UserRPCReturn<std::optional<bool>> update_user(const std::string& session_id, const std::string& username, const std::string& nickname);

        /**
         * @brief delete_account
         * @param session_id
         * @return ( bool | std::nullopt ) + message
         */
        UserRPCReturn<std::optional<bool>> delete_account(const std::string& session_id);

    private:
        // UserService服务的存根，用于发起RPC调用
        std::unique_ptr<user::v1::UserService::UserService::Stub> stub_;
    };

}

#endif //USERRPC_H
