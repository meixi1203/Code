/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "request_manager.h"
#include "callback_manager.h"
#include "data_manager.h"
#include "common.h"
#include "md5.h"
#include "key_center.h"
#include "zmq/zmq.hpp"
#include "log.h"
#include <thread>
#include <unistd.h>

namespace walletfront {

RequestManager::RequestManager() : m_pCallBack(NULL)
{

}

RequestManager::~RequestManager()
{
    if(m_pCallBack)
    {
        delete m_pCallBack;
        m_pCallBack = NULL;
    }
}

void RequestManager::RequestManagerInit()
{
    std::thread heart_thread(&RequestManager::HeartBeatHandler, this);
    heart_thread.detach();
    std::thread recv_thread(&RequestManager::Run, this);
    recv_thread.detach();
    std::thread recv_thread_(&RequestManager::Run, this);
    recv_thread_.detach();

    m_pCallBack = new CallBackManager();
    if(m_pCallBack)
    {
        m_pCallBack->CallBackManagerInit();
    }

}

void RequestManager::Run()
{
    while(1)
    {
        int socket = -1;
        char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
        //INFO_LOG("Recv buff size  = " <<DataManager::GetInstance()->RecvQueueSize());
        DataManager::GetInstance()->RecvQueueBack(socket, sender_msg);
        if(!sender_msg)
        {
            break;
        }
        else if(CheckMd5(sender_msg))
        {
            OnRequest(socket, sender_msg);
        }
    }
}

void RequestManager::OnRequest(const int &socket, const char *message)
{
    if(message == NULL)
    {
        ERROR_LOG("message == NULL");
        return;
    }

    FrontEngine::RequestMessage req_message;
    req_message.ParseFromArray(message, MESSAGE_BODY_SIZE);
    
    INFO_LOG("message type = " << req_message.type());
    INFO_LOG("message client_id = " << req_message.client_id());

    if(req_message.client_id().empty() && req_message.type() != FrontEngine::enums_RequestType::enums_RequestType_CheckAppId)
    {
        //INFO_LOG(" sdk id is empty");
    }
    if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_CheckAppId)
    {
        CheckAppId(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_HeartBeatRequest)
    {
        RequestHeartBeat(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_CreateAccountRequest)
    {
        RequestCreatAccount(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryBalanceRequest)
    {
        RequestQueryBalance(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryFeeRequest)
    {
        QueryFeeRequest(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryOrderRequest)
    {
        RequestQueryOrder(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryOrdersRequest)
    {
        RequestQueryOrders(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_SendCoinRequest)
    {
        RequestSendCoin(req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_RecieveCoinRequest)
    {
        RequestRecieveCoin(req_message);
    }
}

void RequestManager::HeartBeatHandler()
{
    while(1)
    {
        m_heat_mtux.lock();
        std::map<std::string,int>::iterator iter =m_heat_count.begin();
        while(iter != m_heat_count.end())
        {
            if(iter->second == 5)
            {
                //TODO:delete sdkid
                INFO_LOG("delete sdkid ");
            }
            else if(iter->second < 5)
            {
                iter->second++;
            }
           
            iter++;
        }
        m_heat_mtux.unlock();
        sleep(3);
    }
}

void RequestManager::SendManage(const char *message)
{
    zmq::context_t context(1);
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.bind(walletfront::Common::GetInstance()->GetPushService().c_str()); // TBD

    int message_size = MESSAGE_BODY_SIZE;
    zmq::message_t *query = new zmq::message_t(message_size);
    memcpy(query->data(), message, message_size);

    sender.send(*query);
    INFO_LOG("wallet front send ok");
}

void RequestManager::CheckAppId(const int &socket, FrontEngine::RequestMessage &req_message)
{
    FrontEngine::CheckAppId* pCheckAppId = req_message.mutable_app_id();
    if(pCheckAppId == NULL)
    {
        ERROR_LOG("pCheckAppId == NULL");
        return;
    }

    KeyCenter key_center;
    key_center.KeyCenterInit();
    bool ret = key_center.CheckAppId(pCheckAppId->app_id());
    INFO_LOG("CheckAppId ret = " << ret);

    FrontEngine::CallBackMessage cb_message;
    cb_message.set_request_id(req_message.request_id());
    cb_message.set_client_id(req_message.client_id());
    cb_message.set_front_id(req_message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_CheckAppIdCallBack);
    cb_message.set_md5("");
    FrontEngine::CheckAppIdCallBack* pCheckAppIdCb = cb_message.mutable_app_id();

    if(ret)  //set sdk id
    {
        if(pCheckAppIdCb)
        {
            pCheckAppIdCb->set_sdk_id("sdk_id");
            pCheckAppIdCb->set_request_id(req_message.request_id());
            pCheckAppIdCb->set_app_id(pCheckAppId->app_id());
        }
        DataManager::GetInstance()->ClientInfoMapUpdate(socket, req_message.request_id());// TODO where fromsdk id
    }
    else  // appId not found
    {
        if(pCheckAppIdCb)
        {
            pCheckAppIdCb->set_sdk_id("");
            pCheckAppIdCb->set_app_id(pCheckAppId->app_id());
        }
    }
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    cb_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SetMd5(sender_msg);

    if(ret)
    {
        DataManager::GetInstance()->CallBackQueuePush(req_message.request_id(), sender_msg);
    }
    else
    {
        DataManager::GetInstance()->CBQueuePush(socket, sender_msg);
    }

    // free pCheckAppId pCheckAppIdCb
    cb_message.release_app_id();
    req_message.release_app_id();
    pCheckAppIdCb = NULL;
    pCheckAppId = NULL;
}

void RequestManager::RequestHeartBeat(FrontEngine::RequestMessage &req_message)
{
    if(req_message.client_id().empty())
    {
        ERROR_LOG("req_message.client_id() is empty");
        return;
    }

    FrontEngine::HeartBeatRequest* pHeatBeat = req_message.mutable_heart_beat();
    if(pHeatBeat == NULL)
    {
        ERROR_LOG("pCheckAppId == NULL");
        return;
    }

    // set count
    SetHeartBeatCount(req_message.client_id());

    //callback
    FrontEngine::CallBackMessage cb_message;
    cb_message.set_request_id(req_message.request_id());
    cb_message.set_client_id(req_message.client_id());
    cb_message.set_front_id(req_message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_HeartBeatCallBack);
    FrontEngine::HeartBeatCallBack* pHeartBeatCb = cb_message.mutable_heart_beat();
    if(pHeartBeatCb)
    {
        pHeartBeatCb->set_data("some data");//TODO
    }

    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    cb_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SetMd5(sender_msg);
    DataManager::GetInstance()->CallBackQueuePush(req_message.request_id(), sender_msg);

    // free pHeatBeat pHeartBeatCb
    cb_message.release_heart_beat();
    req_message.release_heart_beat();
    pHeatBeat = NULL;
    pHeartBeatCb = NULL;
}

void RequestManager::RequestCreatAccount(FrontEngine::RequestMessage &req_message)
{
    if(req_message.client_id().empty())
    {
        ERROR_LOG("req_message.client_id() is empty");
        return;
    }

    FrontEngine::CreateAccountRequest* pcreat_account = req_message.mutable_account();
    if(pcreat_account == NULL)
    {
        ERROR_LOG("pCheckAppId == NULL");
        return;
    }

    INFO_LOG("CreateAccountRequest request_id = " << pcreat_account->request_id());
    INFO_LOG("CreateAccountRequest username = " << pcreat_account->username());
    INFO_LOG("CreateAccountRequest password = " << pcreat_account->password());
    INFO_LOG("CreateAccountRequest coin_type = " << pcreat_account->coin_type());

    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    // free pHeatBeat pHeartBeatCb
    req_message.release_account();
    pcreat_account = NULL;
}

void RequestManager::RequestQueryBalance(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

}

void RequestManager::QueryFeeRequest(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);
}

void RequestManager::RequestQueryOrder(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);
}

void RequestManager::RequestQueryOrders(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);
}

void RequestManager::RequestSendCoin(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);
}

void RequestManager::RequestRecieveCoin(FrontEngine::RequestMessage &req_message)
{
    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.ParseFromArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);
}

bool RequestManager::CheckMd5(char *message)
{
    bool ret = false;
    FrontEngine::RequestMessage req_message;
    req_message.ParseFromArray(message, MESSAGE_BODY_SIZE);

    std::string client_md5("");
    client_md5 = req_message.md5();

    req_message.set_md5("");
    memset(message, 0, MESSAGE_BODY_SIZE);
    req_message.SerializeToArray(message,MESSAGE_BODY_SIZE);

    std::string current_md5("");
    MD5 md5(message, MESSAGE_BODY_SIZE);
    current_md5 = md5.toString();

    if(client_md5 == current_md5)
    {
        ret = true;
    }
    else
    {
        ERROR_LOG(" This socket data packet is incomplete");
    }

    return ret;
}

bool RequestManager::SetMd5(char *message)
{
    std::string current_md5("");
    MD5 md5(message, MESSAGE_BODY_SIZE);
    current_md5 = md5.toString();

    FrontEngine::CallBackMessage cb_message;
    cb_message.ParseFromArray(message, MESSAGE_BODY_SIZE);
    cb_message.set_md5(current_md5);

    memset(message, 0, MESSAGE_BODY_SIZE);
    cb_message.SerializeToArray(message, MESSAGE_BODY_SIZE);

    return true;
}

bool RequestManager::SetHeartBeatCount(const std::string &sdk_id)
{
    m_heat_mtux.lock();
    m_heat_count[sdk_id] = 0;
    m_heat_mtux.unlock();
}
}
