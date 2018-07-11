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
    m_reqfun_vec.push_back(std::bind(&SendManager::CheckAppid_No, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::CheckAppid_Ok, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqHeartBeat, this));
    m_reqfun_vec.push_back(std::bind(&SendManager::ReqHeartBeat, this));
    //m_reqfun_vec.push_back(std::bind(&SendManager::ReqCreatAccount, this));
}

void SendManager::run()
{
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
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id("client_id");
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
    req_message.set_request_id(ChangeIntToStr());
    req_message.set_client_id("client_id");
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
}
