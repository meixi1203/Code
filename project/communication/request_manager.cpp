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
#include <sstream>

#define FORTESTS

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

    INFO_LOG("message type = " << req_message.type() << " client_id = " << req_message.client_id());

    if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_CheckAppId)
    {
        CheckAppId(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_HeartBeatRequest)
    {
        RequestHeartBeat(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_CreateAccountRequest)
    {
        RequestCreatAccount(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryBalanceRequest)
    {
        RequestQueryBalance(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryFeeRequest)
    {
        QueryFeeRequest(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryOrderRequest)
    {
        RequestQueryOrder(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_QueryOrdersRequest)
    {
        RequestQueryOrders(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_SendCoinRequest)
    {
        RequestSendCoin(socket, req_message);
    }
    else if(req_message.type() == FrontEngine::enums_RequestType::enums_RequestType_RecieveCoinRequest)
    {
        RequestRecieveCoin(socket, req_message);
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
            std::string sdk_id = iter->first;
            if(iter->second == 5)
            {
                DataManager::GetInstance()->ClientInfoMapErase(sdk_id);
                if(RemoveSdkId(sdk_id))
                {
                    INFO_LOG("delete sdkid " << sdk_id);
                    break;
                }
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
#ifndef FORTESTS
    zmq::context_t context(1);
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.bind(walletfront::Common::GetInstance()->GetPushService().c_str()); // TBD

    int message_size = MESSAGE_BODY_SIZE;
    zmq::message_t *query = new zmq::message_t(message_size);
    memcpy(query->data(), message, message_size);

    sender.send(*query);
    INFO_LOG("wallet front send ok");
#endif
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
            pCheckAppIdCb->set_sdk_id(MakeSdkId(pCheckAppId->app_id(), socket));
            pCheckAppIdCb->set_request_id(req_message.request_id());
            pCheckAppIdCb->set_app_id(pCheckAppId->app_id());
        }
        DataManager::GetInstance()->ClientInfoMapUpdate(socket, pCheckAppIdCb->sdk_id());// TODO where from sdk id
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

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(sender_msg);
    }

    if(ret)
    {
        DataManager::GetInstance()->CallBackQueuePush(pCheckAppIdCb->sdk_id(), sender_msg);
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

void RequestManager::RequestHeartBeat(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::HeartBeatRequest* pHeatBeat = req_message.mutable_heart_beat();
    if(pHeatBeat == NULL)
    {
        ERROR_LOG("pHeatBeat == NULL");
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
    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(sender_msg);
    }

    DataManager::GetInstance()->CallBackQueuePush(req_message.client_id(), sender_msg);

    // free pHeatBeat pHeartBeatCb
    cb_message.release_heart_beat();
    req_message.release_heart_beat();
    pHeatBeat = NULL;
    pHeartBeatCb = NULL;
}

bool RequestManager::RemoveSdkId(const std::string &sdk_id)
{
    bool bRet = false;
    std::map<std::string, int>::iterator iter;
    iter = m_heat_count.find(sdk_id);

    if(iter != m_heat_count.end())
    {
        m_heat_count.erase(iter);
        bRet = true;
    }
    return bRet;
}

std::string RequestManager::MakeSdkId(const std::string &sdk_id, const int &socket)
{
    std::string sdk = sdk_id;
    sdk += "-";

    std::ostringstream oss;
    oss << socket;
    sdk += oss.str();

    return sdk;
}

void RequestManager::RequestCreatAccount(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::CreateAccountRequest* pCreatAccount = req_message.mutable_account();
    if(pCreatAccount == NULL)
    {
        ERROR_LOG("pCreatAccount == NULL");
        return;
    }

    INFO_LOG("CreateAccountRequest request_id = " << pCreatAccount->request_id());
    INFO_LOG("CreateAccountRequest username = " << pCreatAccount->username());
    INFO_LOG("CreateAccountRequest password = " << pCreatAccount->password());
    INFO_LOG("CreateAccountRequest coin_type = " << pCreatAccount->coin_type());

    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    // free pHeatBeat pHeartBeatCb
    req_message.release_account();
    pCreatAccount = NULL;

#ifdef FORTESTS
    CreatAccountCallBack(req_message); // for tests IF
#endif
}

void RequestManager::RequestQueryBalance(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::QueryBalanceRequest* pQueryBlance = req_message.mutable_balance();
    if(pQueryBlance == NULL)
    {
        ERROR_LOG("pQueryBlance == NULL");
        return;
    }

    INFO_LOG("QueryBalanceRequest request_id = " << pQueryBlance->request_id());
    INFO_LOG("QueryBalanceRequest address = " << pQueryBlance->address());
    INFO_LOG("QueryBalanceRequest coin_type = " << pQueryBlance->coin_type());

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_balance();
    pQueryBlance = NULL;

#ifdef FORTESTS
    QueryBalanceCallBack(req_message); // for tests IF
#endif
}

void RequestManager::QueryFeeRequest(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::QueryFeeRequest* pQueryFee = req_message.mutable_fee();
    if(pQueryFee == NULL)
    {
        ERROR_LOG("pQueryFee == NULL");
        return;
    }

    INFO_LOG("QueryFeeRequest request_id = " << pQueryFee->request_id());
    INFO_LOG("QueryFeeRequest coin_type = " << pQueryFee->coin_type());

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_fee();
    pQueryFee = NULL;

#ifdef FORTESTS
    QueryFeeCallBack(req_message); // for tests IF
#endif
}

void RequestManager::RequestQueryOrder(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::QueryOrderRequest* pQueryOrder = req_message.mutable_order();
    if(pQueryOrder == NULL)
    {
        ERROR_LOG("pQueryOrder == NULL");
        return;
    }

    INFO_LOG("QueryOrderRequest request_id = " << pQueryOrder->request_id());
    INFO_LOG("QueryOrderRequest coin_type = " << pQueryOrder->coin_type());
    INFO_LOG("QueryOrderRequest order_id = " << pQueryOrder->order_id());

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_order();
    pQueryOrder = NULL;

#ifdef FORTESTS
    QueryOrderCallBack(req_message); // for tests IF
#endif
}

void RequestManager::RequestQueryOrders(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::QueryOrdersRequest* pQueryOrders = req_message.mutable_orders();
    if(pQueryOrders == NULL)
    {
        ERROR_LOG("pCheckAppId == NULL");
        return;
    }

    INFO_LOG("QueryOrdersRequest request_id = " << pQueryOrders->request_id());
    INFO_LOG("QueryOrdersRequest coin_type = " << pQueryOrders->coin_type());
    INFO_LOG("QueryOrdersRequest address = " << pQueryOrders->address());
    FrontEngine::Order* pOrder = pQueryOrders->mutable_order();
    if(pOrder == NULL)
    {
        ERROR_LOG("pOrder == NULL");
        return;
    }

    INFO_LOG("QueryOrdersRequest order_id = " << pOrder->order_id());
    INFO_LOG("QueryOrdersRequest type = " << pOrder->type());
    INFO_LOG("QueryOrdersRequest from_address = " << pOrder->from_address());
    INFO_LOG("QueryOrdersRequest to_address = " << pOrder->to_address());
    INFO_LOG("QueryOrdersRequest description = " << pOrder->description());
    INFO_LOG("QueryOrdersRequest amount = " << pOrder->amount());
    INFO_LOG("QueryOrdersRequest fee = " << pOrder->fee());
    INFO_LOG("QueryOrdersRequest order_date = " << pOrder->order_date());
    INFO_LOG("QueryOrdersRequest order_time = " << pOrder->order_time());
    INFO_LOG("QueryOrdersRequest status = " << pOrder->status());
    pQueryOrders->release_order();
    pOrder = NULL;

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_orders();
    pQueryOrders = NULL;

#ifdef FORTESTS
    QueryOrdersCallBack(req_message); // for tests IF
#endif
}

void RequestManager::RequestSendCoin(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::SendCoinRequest* pSendCoin = req_message.mutable_send_info();
    if(pSendCoin == NULL)
    {
        ERROR_LOG("pSendCoin == NULL");
        return;
    }

    INFO_LOG("RequestSendCoin request_id = " << pSendCoin->request_id());
    INFO_LOG("RequestSendCoin coin_type = " << pSendCoin->coin_type());
    INFO_LOG("RequestSendCoin from_address = " << pSendCoin->from_address());
    INFO_LOG("RequestSendCoin from_pwd = " << pSendCoin->from_pwd());
    INFO_LOG("RequestSendCoin to_address = " << pSendCoin->to_address());
    INFO_LOG("RequestSendCoin description = " << pSendCoin->description());
    INFO_LOG("RequestSendCoin amount = " << pSendCoin->amount());
    FrontEngine::Order* pOrder = pSendCoin->mutable_order();
    if(pOrder == NULL)
    {
        ERROR_LOG("pOrder == NULL");
        return;
    }

    INFO_LOG("RequestSendCoin order_id = " << pOrder->order_id());
    INFO_LOG("RequestSendCoin type = " << pOrder->type());
    INFO_LOG("RequestSendCoin from_address = " << pOrder->from_address());
    INFO_LOG("RequestSendCoin to_address = " << pOrder->to_address());
    INFO_LOG("RequestSendCoin description = " << pOrder->description());
    INFO_LOG("RequestSendCoin amount = " << pOrder->amount());
    INFO_LOG("RequestSendCoin fee = " << pOrder->fee());
    INFO_LOG("RequestSendCoin order_date = " << pOrder->order_date());
    INFO_LOG("RequestSendCoin order_time = " << pOrder->order_time());
    INFO_LOG("RequestSendCoin status = " << pOrder->status());
    pSendCoin->release_order();
    pOrder = NULL;

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_send_info();
    pSendCoin = NULL;

#ifdef FORTESTS
    TradedCallBack(req_message); // for tests IF
#endif
}

void RequestManager::RequestRecieveCoin(const int &socket, FrontEngine::RequestMessage &req_message)
{
    if(!CheckClientId(socket, req_message))
    {
        return;
    }
    FrontEngine::RecieveCoinRequest* pRecieveCoin = req_message.mutable_recieve_info();
    if(pRecieveCoin == NULL)
    {
        ERROR_LOG("pRecieveCoin == NULL");
        return;
    }

    INFO_LOG("RequestRecieveCoin request_id = " << pRecieveCoin->request_id());
    INFO_LOG("RequestRecieveCoin coin_type = " << pRecieveCoin->coin_type());
    INFO_LOG("RequestRecieveCoin from_address = " << pRecieveCoin->from_address());
    INFO_LOG("RequestRecieveCoin from_pwd = " << pRecieveCoin->from_pwd());
    INFO_LOG("RequestRecieveCoin to_address = " << pRecieveCoin->to_address());
    INFO_LOG("RequestRecieveCoin description = " << pRecieveCoin->description());
    INFO_LOG("RequestRecieveCoin amount = " << pRecieveCoin->amount());
    FrontEngine::Order* pOrder = pRecieveCoin->mutable_order();
    if(pOrder == NULL)
    {
        ERROR_LOG("pOrder == NULL");
        return;
    }

    INFO_LOG("RequestRecieveCoin order_id = " << pOrder->order_id());
    INFO_LOG("RequestRecieveCoin type = " << pOrder->type());
    INFO_LOG("RequestRecieveCoin from_address = " << pOrder->from_address());
    INFO_LOG("RequestRecieveCoin to_address = " << pOrder->to_address());
    INFO_LOG("RequestRecieveCoin description = " << pOrder->description());
    INFO_LOG("RequestRecieveCoin amount = " << pOrder->amount());
    INFO_LOG("RequestRecieveCoin fee = " << pOrder->fee());
    INFO_LOG("RequestRecieveCoin order_date = " << pOrder->order_date());
    INFO_LOG("RequestRecieveCoin order_time = " << pOrder->order_time());
    INFO_LOG("RequestRecieveCoin status = " << pOrder->status());
    pRecieveCoin->release_order();
    pOrder = NULL;

    //TODO
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    req_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);
    SendManage(sender_msg);

    req_message.release_recieve_info();
    pRecieveCoin = NULL;

#ifdef FORTESTS
    TradedCallBack(req_message); // for tests IF
#endif
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

bool RequestManager::SetHeartBeatCount(const std::string &sdk_id)
{
    m_heat_mtux.lock();
    m_heat_count[sdk_id] = 0;
    m_heat_mtux.unlock();
}

bool RequestManager::CheckClientId(const int &socket, const FrontEngine::RequestMessage &req_message)
{
    if(!req_message.client_id().empty())
    {
        return true;
    }

    std::map<FrontEngine::enums_RequestType, FrontEngine::enums_CallBackType> callback_map;
    callback_map[FrontEngine::enums_RequestType_CreateAccountRequest] = FrontEngine::enums_CallBackType_CreateAccountCallBack;
    callback_map[FrontEngine::enums_RequestType_QueryBalanceRequest] = FrontEngine::enums_CallBackType_QueryBalanceCallBack;
    callback_map[FrontEngine::enums_RequestType_QueryFeeRequest] = FrontEngine::enums_CallBackType_QueryFeeCallBack;
    callback_map[FrontEngine::enums_RequestType_QueryOrderRequest] = FrontEngine::enums_CallBackType_QueryOrderCallBack;
    callback_map[FrontEngine::enums_RequestType_QueryOrdersRequest] = FrontEngine::enums_CallBackType_QueryOrdersCallBack;
    callback_map[FrontEngine::enums_RequestType_SendCoinRequest] = FrontEngine::enums_CallBackType_TradedCallBack;
    callback_map[FrontEngine::enums_RequestType_RecieveCoinRequest] = FrontEngine::enums_CallBackType_TradedCallBack;
    callback_map[FrontEngine::enums_RequestType_HeartBeatRequest] = FrontEngine::enums_CallBackType_HeartBeatCallBack;

    FrontEngine::CallBackMessage cb_message;
    cb_message.set_client_id(req_message.client_id());
    cb_message.set_request_id(req_message.request_id());
    cb_message.set_front_id(req_message.front_id());
    cb_message.set_type(callback_map[req_message.type()]);

    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
    cb_message.SerializeToArray(sender_msg, MESSAGE_BODY_SIZE);

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(sender_msg);
    }

    DataManager::GetInstance()->CBQueuePush(socket, sender_msg);
    return false;
}

void RequestManager::CreatAccountCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_CreateAccountCallBack);

    FrontEngine::CreateAccountCallBack* pCreatAccount = cb_message.mutable_account();
    FrontEngine::UserInfo* pUserInfo = pCreatAccount->mutable_account();

    if(pCreatAccount)
    {
        pCreatAccount->set_request_id("pCreatAccount->set_request_id");
        pCreatAccount->set_errorcode(1001);
        pCreatAccount->set_error("pCreatAccount->set_error");
        if(pUserInfo)
        {
            pUserInfo->set_type(FrontEngine::enums_CoinType::enums_CoinType_BTC);
            pUserInfo->set_username("pUserInfo->set_username");
            pUserInfo->set_password("pUserInfo->set_password");
            pUserInfo->set_address("pUserInfo->set_address");
        }
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    pCreatAccount->release_account();
    pUserInfo = NULL;
    cb_message.release_account();
    pCreatAccount = NULL;
}

void RequestManager::QueryBalanceCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryBalanceCallBack);

    FrontEngine::QueryBalanceCallBack* pQueryBalance = cb_message.mutable_balance();
    FrontEngine::CoinCapital* pCoinCapital = pQueryBalance->mutable_capital();

    if(pQueryBalance)
    {
        pQueryBalance->set_request_id("QueryBalance->request_id");
        pQueryBalance->set_errorcode(1002);
        pQueryBalance->set_error("pQueryBalance->error");
        if(pCoinCapital)
        {
            pCoinCapital->set_address("pCoinCapital->address");
            pCoinCapital->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pCoinCapital->set_amount("pCoinCapital->amount");
            pCoinCapital->set_frozen_amount("pCoinCapital->frozen_amount");
        }
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    pQueryBalance->release_capital();
    pCoinCapital = NULL;
    cb_message.release_balance();
    pQueryBalance = NULL;
}

void RequestManager::QueryFeeCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryFeeCallBack);

    FrontEngine::QueryFeeCallBack* pQueryFee = cb_message.mutable_fee();

    if(pQueryFee)
    {
        pQueryFee->set_request_id("pQueryFee->request_id");
        pQueryFee->set_coin_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
        pQueryFee->set_fee("pQueryFee->fee");
        pQueryFee->set_errorcode(1003);
        pQueryFee->set_error("pQueryFee->error");
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    cb_message.release_fee();
    pQueryFee = NULL;
}

void RequestManager::QueryOrderCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrderCallBack);

    FrontEngine::QueryOrderCallBack* pQueryOrder = cb_message.mutable_order();
    FrontEngine::Order* pOrder = pQueryOrder->mutable_order();

    if(pQueryOrder)
    {
        pQueryOrder->set_request_id("pQueryOrder->request_id");
        pQueryOrder->set_errorcode(1004);
        pQueryOrder->set_error("pQueryOrder->error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    pQueryOrder->release_order();
    pOrder = NULL;
    cb_message.release_order();
    pQueryOrder = NULL;
}

void RequestManager::QueryOrdersCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrdersCallBack);

    FrontEngine::QueryOrdersCallBack* pQueryOrders = cb_message.mutable_orders();
    FrontEngine::Order* pOrder = NULL;
    if(pQueryOrders)
    {
        pQueryOrders->set_request_id("pQueryOrders->request_id");
        pQueryOrders->set_errorcode(1005);
        pQueryOrders->set_error("pQueryOrders->error");

        for(int index = 0; index < 5; index++)
        {
            pOrder = pQueryOrders->add_order();
            if(pOrder)
            {
                pOrder->set_order_id("pOrder->order_id");
                pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
                pOrder->set_from_address("pOrder->from_address");
                pOrder->set_to_address("pOrder->to_address");
                pOrder->set_description("pOrder->description");
                pOrder->set_amount("pOrder->amount");
                pOrder->set_fee("pOrder->fee");
                pOrder->set_order_date(20170713);
                pOrder->set_order_time(20170713);
                pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
            }
        }
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    cb_message.release_orders();
    pQueryOrders = NULL;
    pOrder = NULL;
}

void RequestManager::TradedCallBack(FrontEngine::RequestMessage &message)
{
    FrontEngine::CallBackMessage cb_message;

    cb_message.set_client_id(message.client_id());
    cb_message.set_request_id(message.request_id());
    cb_message.set_front_id(message.front_id());
    cb_message.set_type(FrontEngine::enums_CallBackType::enums_CallBackType_TradedCallBack);

    FrontEngine::TradedCallBack* pTraded = cb_message.mutable_traded_info();
    FrontEngine::Order* pOrder = pTraded->mutable_order();

    if(pTraded)
    {
        pTraded->set_request_id("pTraded->request_id");
        pTraded->set_errorcode(1006);
        pTraded->set_error("pTraded->error");
        if(pOrder)
        {
            pOrder->set_order_id("pOrder->order_id");
            pOrder->set_type(FrontEngine::enums_CoinType::enums_CoinType_ETH);
            pOrder->set_from_address("pOrder->from_address");
            pOrder->set_to_address("pOrder->to_address");
            pOrder->set_description("pOrder->description");
            pOrder->set_amount("pOrder->amount");
            pOrder->set_fee("pOrder->fee");
            pOrder->set_order_date(20170713);
            pOrder->set_order_time(20170713);
            pOrder->set_status(FrontEngine::enums_OrderStatus::enums_OrderStatus_TO_BE_REPORTED);
        }
    }

    if(m_pCallBack)
    {
        m_pCallBack->SetMd5(cb_message);
        m_pCallBack->SerializeToArray(cb_message);
    }

    pTraded->release_order();
    cb_message.release_traded_info();
    pTraded = NULL;
    pOrder = NULL;
}

}
