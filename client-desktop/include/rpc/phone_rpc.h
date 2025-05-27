#ifndef PHONE_RPC_H
#define PHONE_RPC_H

#include <string>
// #include <map>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "phone.grpc.pb.h"
#include "mobile_phone.h"

namespace zinpass::rpc{

class PhoneRPC{
public:
    template<typename T>
    struct Return{
        T data;
        std::string message;
    };

    // 1、constructor, 接受一个gRPC通道的共享指针来初始化存根
    PhoneRPC(std::shared_ptr<grpc::Channel> channel);

    // 2、RPC service
    /**
     * @brief add_phone
     * @param session_id
     * @param telecom_operator
     * @param phone_area
     * @param phone_number
     * @param pin
     * @param puk
     * @param service_password
     * @param join_time
     * @param postscript
     * @return
     */
    Return<bool> add_phone(
        const std::string& session_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript);


    /**
     * @brief list_phone_numbers
     * @param session_id
     * @return phone_numbers + message
     */
    Return<std::vector<std::string>> list_phone_numbers(const std::string& session_id);

    /**
     * @brief list_phones
     * @param session_id
     * @param telecoms
     * @param phone_numbers
     * @return
     */
    Return<std::vector<zinpass::models::MobilePhone>> list_phones(const std::string& session_id, const std::string& telecom, const std::string& phone_number);

    /**
     * @brief find_phone_by_id
     * @param session_id
     * @param phone_id
     * @return
     */
    Return<zinpass::models::MobilePhone> find_phone_by_id(const std::string& session_id, int phone_id);

    /**
     * @brief update_phone_by_id
     * @param session_id
     * @param phone_id
     * @param telecom_operator
     * @param phone_area
     * @param phone_number
     * @param pin
     * @param puk
     * @param service_password
     * @param join_time
     * @param postscript
     * @return
     */
    Return<bool> update_phone_by_id(
        const std::string& session_id,
        int phone_id,
        const std::string& telecom_operator,
        const std::string& phone_area,
        const std::string& phone_number,
        const std::string& pin,
        const std::string& puk,
        const std::string& service_password,
        const std::string& join_time,
        const std::string& postscript
        );

    /**
     * @brief delete_phone_by_id
     * @param session_id
     * @param phone_id
     * @return
     */
    Return<bool> delete_phone_by_id(const std::string& session_id, int phone_id);

private:
    // PhoneService服务的存根
    std::unique_ptr<account::v1::PhoneService::PhoneService::Stub> stub_;
};

}

#endif
