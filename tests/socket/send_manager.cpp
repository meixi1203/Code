/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "send_manager.h"
#include "front_engine.pb.h"
#include "data_manager.h"
#include "md5.h"
#include <thread>
#include <unistd.h>
#include <iostream>
#include <sstream>

#define STRING_EMPTY ""
#define MESSAGE_BODY_SIZE 4096

namespace walletfront {

SendManager::SendManager()
{

}
SendManager::~SendManager()
{

}

void SendManager::SendManagerInit()
{
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqCreatAccount, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqQueryBalance, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqQueryFee, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqQueryOrder, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqQueryOrders, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqSendCoin, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqRecieveCoin, this));

    std::thread heart_thread(&SendManager::HeartBeatHandler, this);
    heart_thread.detach();
}

void SendManager::HeartBeatHandler()
{
    while(1)
    {
        ReqHeartBeat();
        sleep(3);
    }
}

void SendManager::run()
{
    CheckAppid_No();
    CheckAppid_Ok();

    while(1)
    {
        for(int v = 0; v < m_reqfun_vec.size(); v++)
        {
            m_reqfun_vec[v]();
            sleep(1);
        }
    }
}

void SendManager::CheckAppid_No()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id("request_id");
    req_message.set_client_id("");
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_CheckAppId);
    FrontEngine::CheckAppId* pCheckAppId = req_message.mutable_app_id();
    if(pCheckAppId)
    {
        pCheckAppId->set_app_id("test 11appid");
        pCheckAppId->set_request_id("request id");
    }
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_app_id();
    pCheckAppId = NULL;

    FrontEngine::RequestMessage req;
    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::CheckAppid_Ok()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id("request_id");
    req_message.set_client_id("");
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_CheckAppId);
    FrontEngine::CheckAppId* pCheckAppId = req_message.mutable_app_id();
    if(pCheckAppId)
    {
        pCheckAppId->set_app_id("06ec8995107752cc9ac0c10914bd6dc2");
        pCheckAppId->set_request_id("request id");
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_app_id();
    pCheckAppId = NULL;

    FrontEngine::RequestMessage req;
    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqHeartBeat()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_HeartBeatRequest);
    FrontEngine::HeartBeatRequest* pHeartBeat = req_message.mutable_heart_beat();
    if(pHeartBeat)
    {
        pHeartBeat->set_request_id("request_id");
        pHeartBeat->set_data("client data");
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_heart_beat();
    pHeartBeat = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);

}
std::string SendManager::ChangeIntToStr()
{
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    std::string stid = oss.str();

    int pid_id = getpid();
    std::ostringstream os;

    os << pid_id;
    stid += oss.str();
    return stid;
}

void SendManager::ReqCreatAccount()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_CreateAccountRequest);
    FrontEngine::CreateAccountRequest* pcreat_account = req_message.mutable_account();
    if(pcreat_account)
    {
        pcreat_account->set_request_id("request_id");
        pcreat_account->set_username("username");
        pcreat_account->set_password("password");
        pcreat_account->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_account();
    pcreat_account = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqQueryBalance()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_QueryBalanceRequest);
    FrontEngine::QueryBalanceRequest* pQueryBlance = req_message.mutable_balance();
    if(pQueryBlance)
    {
        pQueryBlance->set_request_id("pQueryBlance->set_request_id");
        pQueryBlance->set_address("pQueryBlance->set_address");
        pQueryBlance->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_balance();
    pQueryBlance = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqQueryFee()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_QueryFeeRequest);
    FrontEngine::QueryFeeRequest* pQueryFee = req_message.mutable_fee();
    if(pQueryFee)
    {
        pQueryFee->set_request_id("pQueryBlance->set_request_id");
        pQueryFee->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_fee();
    pQueryFee = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqQueryOrder()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_QueryOrderRequest);
    FrontEngine::QueryOrderRequest* pQueryOrder = req_message.mutable_order();
    if(pQueryOrder)
    {
        pQueryOrder->set_request_id("pQueryBlance->set_request_id");
        pQueryOrder->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pQueryOrder->set_order_id("pQueryOrder->set_order_id");
    }

    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    req_message.release_order();
    pQueryOrder = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqQueryOrders()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_QueryOrdersRequest);
    FrontEngine::QueryOrdersRequest* pQueryOrders = req_message.mutable_orders();
    if(pQueryOrders)
    {
        pQueryOrders->set_request_id("pQueryBlance->set_request_id");
        pQueryOrders->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pQueryOrders->set_address("pQueryOrder->set_address");
    }

    FrontEngine::Order* pOrder = pQueryOrders->mutable_order();
    if(pOrder)
    {
        pOrder->set_order_id("pOrder->set_order_id");
        pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pOrder->set_from_address("pOrder->set_from_address");
        pOrder->set_to_address("pOrder->set_to_address");
        pOrder->set_description("pOrder->set_description");
        pOrder->set_amount("pOrder->set_amount");
        pOrder->set_fee("pOrder->set_fee");
        pOrder->set_order_date(201807012);
        pOrder->set_order_time(201807012);
        pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
    }
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    pQueryOrders->release_order();
    pOrder = NULL;
    req_message.release_orders();
    pQueryOrders = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqSendCoin()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_SendCoinRequest);
    FrontEngine::SendCoinRequest* pSendCoin = req_message.mutable_send_info();
    if(pSendCoin)
    {
        pSendCoin->set_request_id("pSendCoin->set_request_id");
        pSendCoin->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pSendCoin->set_from_address("pSendCoin->from_address");
        pSendCoin->set_from_pwd("pSendCoin->from_pwd");
        pSendCoin->set_to_address("pSendCoin->to_address");
        pSendCoin->set_description("pSendCoin->description");
        pSendCoin->set_amount("endCoin->amount");
    }

    FrontEngine::Order* pOrder = pSendCoin->mutable_order();
    if(pOrder)
    {
        pOrder->set_order_id("pOrder->set_order_id");
        pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pOrder->set_from_address("pOrder->set_from_address");
        pOrder->set_to_address("pOrder->set_to_address");
        pOrder->set_description("pOrder->set_description");
        pOrder->set_amount("pOrder->set_amount");
        pOrder->set_fee("pOrder->set_fee");
        pOrder->set_order_date(201807012);
        pOrder->set_order_time(201807012);
        pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
    }
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    pSendCoin->release_order();
    pOrder = NULL;
    req_message.release_send_info();
    pSendCoin = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}

void SendManager::ReqRecieveCoin()
{
    char sender_msg[MESSAGE_BODY_SIZE] = "";

    FrontEngine::RequestMessage req_message;
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id(DataManager::GetInstance()->GetSdkId());
    req_message.set_front_id("front_id");
    req_message.set_md5(STRING_EMPTY);
    req_message.set_type(FrontEngine::enums_RequestType::enums_RequestType_RecieveCoinRequest);
    FrontEngine::RecieveCoinRequest* pRecieveCoin = req_message.mutable_recieve_info();
    if(pRecieveCoin)
    {
        pRecieveCoin->set_request_id("pSendCoin->set_request_id");
        pRecieveCoin->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pRecieveCoin->set_from_address("pSendCoin->from_address");
        pRecieveCoin->set_from_pwd("pSendCoin->from_pwd");
        pRecieveCoin->set_to_address("pSendCoin->to_address");
        pRecieveCoin->set_description("pSendCoin->description");
        pRecieveCoin->set_amount("endCoin->amount");
    }

    FrontEngine::Order* pOrder = pRecieveCoin->mutable_order();
    if(pOrder)
    {
        pOrder->set_order_id("pOrder->set_order_id");
        pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
        pOrder->set_from_address("pOrder->set_from_address");
        pOrder->set_to_address("pOrder->set_to_address");
        pOrder->set_description("pOrder->set_description");
        pOrder->set_amount("pOrder->set_amount");
        pOrder->set_fee("pOrder->set_fee");
        pOrder->set_order_date(201807012);
        pOrder->set_order_time(201807012);
        pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
    }
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    pRecieveCoin->release_order();
    pOrder = NULL;
    req_message.release_recieve_info();
    pRecieveCoin = NULL;

    MD5 md5(sender_msg, MESSAGE_BODY_SIZE);
    std::string sts_md5 = md5.toString();

    FrontEngine::RequestMessage req;
    req.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    req.set_md5 (sts_md5);
    memset(sender_msg, 0, MESSAGE_BODY_SIZE);
    req.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    DataManager::GetInstance()->ReqQueuePush(sender_msg);
}
}
