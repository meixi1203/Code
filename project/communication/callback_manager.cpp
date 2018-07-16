/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "callback_manager.h"
#include "data_manager.h"
#include "common.h"
#include "zmq/zmq.hpp"
#include "log.h"
#include "md5.h"
#include <thread>

namespace walletfront {

CallBackManager::CallBackManager()
{

}

CallBackManager::~CallBackManager()
{

}

void CallBackManager::CallBackManagerInit()
{
    std::thread callback_thread(&CallBackManager::ZmqSub, this);
    callback_thread.detach();
}

void CallBackManager::ZmqSub()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(walletfront::Common::GetInstance()->GetPubService().c_str());
    const char *filter = "";

    subscriber.setsockopt( ZMQ_SUBSCRIBE, filter, strlen(filter));

    while(1)
    {
        zmq::message_t rev;
        subscriber.recv(&rev);
        if(rev.size() == MESSAGE_BODY_SIZE && rev.data() != NULL)
        {
            SetMd5((char *)rev.data());
            OnResponse((char *)rev.data());
        }
    }
}

void CallBackManager::OnResponse(const char *message)
{
    if(message == NULL)
    {
        ERROR_LOG("OnResponse message == NULL");
        return;
    }

    FrontEngine::CallBackMessage cb_message;
    cb_message.ParseFromArray(message, MESSAGE_BODY_SIZE);

    if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_CreateAccountCallBack)
    {
        CreatAccountCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_QueryFeeCallBack)
    {
        QueryFeeCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_QueryBalanceCallBack)
    {
        QueryBalanceCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrderCallBack)
    {
        QueryOrderCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_QueryOrdersCallBack)
    {
        QueryOrdersCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_TradedCallBack)
    {
        TradedCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_CanceledCallBack)
    {
        CanceledCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_RejectedCallBack)
    {
        RejectedCallBack(cb_message);
    }
    else
    {
        ERROR_LOG(" Error message type  " << cb_message.type());
    }

}

void CallBackManager::CreatAccountCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("CreatAccountCallBack client_id = " << cb_message.client_id());
    FrontEngine::CreateAccountCallBack* pCreatAccount = cb_message.mutable_account();
    FrontEngine::UserInfo* pUserInfo = pCreatAccount->mutable_account();

    if(pCreatAccount)
    {
        INFO_LOG("CreatAccountCallBack request_id = " << pCreatAccount->request_id());
        INFO_LOG("CreatAccountCallBack errorCode = " << pCreatAccount->errorcode());
        INFO_LOG("CreatAccountCallBack error = " << pCreatAccount->error());
        if(pUserInfo)
        {
            INFO_LOG("CreatAccountCallBack type = " << pUserInfo->type());
            INFO_LOG("CreatAccountCallBack username = " << pUserInfo->username());
            INFO_LOG("CreatAccountCallBack password = " << pUserInfo->password());
            INFO_LOG("CreatAccountCallBack address = " << pUserInfo->address());
        }
    }

    SerializeToArray(cb_message);
    pCreatAccount->release_account();
    pUserInfo = NULL;
    cb_message.release_account();
    pCreatAccount = NULL;
}

void CallBackManager::QueryBalanceCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("QueryBalanceCallBack client_id = " << cb_message.client_id());
    FrontEngine::QueryBalanceCallBack* pQueryBalance = cb_message.mutable_balance();
    FrontEngine::CoinCapital* pCoinCapital = pQueryBalance->mutable_capital();

    if(pQueryBalance)
    {
        INFO_LOG("QueryBalanceCallBack request_id = " << pQueryBalance->request_id());
        INFO_LOG("QueryBalanceCallBack errorCode = " << pQueryBalance->errorcode());
        INFO_LOG("QueryBalanceCallBack error = " << pQueryBalance->error());
        if(pCoinCapital)
        {
           INFO_LOG("QueryBalanceCallBack address = " << pCoinCapital->address());
           INFO_LOG("QueryBalanceCallBack type = " << pCoinCapital->type());
           INFO_LOG("QueryBalanceCallBack amount = " << pCoinCapital->amount());
           INFO_LOG("QueryBalanceCallBack frozen_amount = " << pCoinCapital->frozen_amount());
        }
    }

    SerializeToArray(cb_message);
    pQueryBalance->release_capital();
    pCoinCapital = NULL;
    cb_message.release_account();
    pQueryBalance = NULL;
}

void CallBackManager::QueryFeeCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("QueryFeeCallBack client_id = " << cb_message.client_id());
    FrontEngine::QueryFeeCallBack* pQueryFee = cb_message.mutable_fee();

    if(pQueryFee)
    {
        INFO_LOG("QueryFeeCallBack request_id = " << pQueryFee->request_id());
        INFO_LOG("QueryFeeCallBack coin_type = " << pQueryFee->coin_type());
        INFO_LOG("QueryFeeCallBack fee = " << pQueryFee->fee());
        INFO_LOG("QueryFeeCallBack errorCode = " << pQueryFee->errorcode()); 
        INFO_LOG("QueryFeeCallBack error = " << pQueryFee->error()); 
    }

    SerializeToArray(cb_message);
    cb_message.release_fee();
    pQueryFee = NULL;
}


void CallBackManager::QueryOrderCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("QueryOrderCallBack client_id = " << cb_message.client_id());
    FrontEngine::QueryOrderCallBack* pQueryOrder = cb_message.mutable_order();
    FrontEngine::Order* pOrder = pQueryOrder->mutable_order();

    if(pQueryOrder)
    {
        INFO_LOG("QueryOrderCallBack request_id = " << pQueryOrder->request_id());
        INFO_LOG("QueryOrderCallBack errorCode = " << pQueryOrder->errorcode());
        INFO_LOG("QueryOrderCallBack error = " << pQueryOrder->error());
        if(pOrder)
        {
            INFO_LOG("QueryOrderCallBack order_id = " << pOrder->order_id());
            INFO_LOG("QueryOrderCallBack type = " << pOrder->type());
            INFO_LOG("QueryOrderCallBack from_address = " << pOrder->from_address());
            INFO_LOG("QueryOrderCallBack to_address = " << pOrder->to_address());
            INFO_LOG("QueryOrderCallBack description = " << pOrder->description());
            INFO_LOG("QueryOrderCallBack amount = " << pOrder->amount());
            INFO_LOG("QueryOrderCallBack fee = " << pOrder->fee());
            INFO_LOG("QueryOrderCallBack order_date = " << pOrder->order_date());
            INFO_LOG("QueryOrderCallBack order_time = " << pOrder->order_time());
            INFO_LOG("QueryOrderCallBack status = " << pOrder->status());
        }
    }

    SerializeToArray(cb_message);
    pQueryOrder->release_order();
    pOrder = NULL;
    cb_message.release_order();
    pQueryOrder = NULL;
}

void CallBackManager::QueryOrdersCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("QueryOrdersCallBack client_id = " << cb_message.client_id());
    FrontEngine::QueryOrdersCallBack* pQueryOrders = cb_message.mutable_orders();

    if(pQueryOrders)
    {
        INFO_LOG("QueryOrdersCallBack request_id = " << pQueryOrders->request_id());
        INFO_LOG("QueryOrdersCallBack errorCode = " << pQueryOrders->errorcode());
        INFO_LOG("QueryOrdersCallBack error = " << pQueryOrders->error());
        INFO_LOG("QueryOrdersCallBack length = " << pQueryOrders->length());
        INFO_LOG("QueryOrdersCallBack order size = " << pQueryOrders->order_size());
        for(int index = 0; index < pQueryOrders->order_size(); index++)
        {
            FrontEngine::Order order = pQueryOrders->order(index);
            INFO_LOG("QueryOrdersCallBack order_id = " << order.order_id());
            INFO_LOG("QueryOrdersCallBack type = " << order.type());
            INFO_LOG("QueryOrdersCallBack from_address = " << order.from_address());
            INFO_LOG("QueryOrdersCallBack to_address = " << order.to_address());
            INFO_LOG("QueryOrdersCallBack description = " << order.description());
            INFO_LOG("QueryOrdersCallBack amount = " << order.amount());
            INFO_LOG("QueryOrdersCallBack fee = " << order.fee());
            INFO_LOG("QueryOrdersCallBack order_date = " << order.order_date());
            INFO_LOG("QueryOrdersCallBack order_time = " << order.order_time());
            INFO_LOG("QueryOrdersCallBack status = " << order.status());
        }
    }

    SerializeToArray(cb_message);
    cb_message.release_orders();
    pQueryOrders = NULL;
}

void CallBackManager::TradedCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("TradedCallBack client_id = " << cb_message.client_id());
    FrontEngine::TradedCallBack* pTraded = cb_message.mutable_traded_info();
    FrontEngine::Order* pOrder = pTraded->mutable_order();

    if(pTraded)
    {
        INFO_LOG("TradedCallBack request_id = " << pTraded->request_id());
        INFO_LOG("TradedCallBack errorCode = " << pTraded->errorcode());
        INFO_LOG("TradedCallBack error = " << pTraded->error());
        if(pOrder)
        {
            INFO_LOG("TradedCallBack order_id = " << pOrder->order_id());
            INFO_LOG("TradedCallBack type = " << pOrder->type());
            INFO_LOG("TradedCallBack from_address = " << pOrder->from_address());
            INFO_LOG("TradedCallBack to_address = " << pOrder->to_address());
            INFO_LOG("TradedCallBack description = " << pOrder->description());
            INFO_LOG("TradedCallBack amount = " << pOrder->amount());
            INFO_LOG("TradedCallBack fee = " << pOrder->fee());
            INFO_LOG("TradedCallBack order_date = " << pOrder->order_date());
            INFO_LOG("TradedCallBack order_time = " << pOrder->order_time());
            INFO_LOG("TradedCallBack status = " << pOrder->status());
        }
    }

    SerializeToArray(cb_message);
    pTraded->release_order();
    pOrder = NULL;
    cb_message.release_traded_info();
    pTraded = NULL;
}

void CallBackManager::CanceledCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("CanceledCallBack client_id = " << cb_message.client_id());
    FrontEngine::CanceledCallBack* pCanceled = cb_message.mutable_canceled_info();
    FrontEngine::Order* pOrder = pCanceled->mutable_order();

    if(pCanceled)
    {
        INFO_LOG("CanceledCallBack request_id = " << pCanceled->request_id());
        INFO_LOG("CanceledCallBack errorCode = " << pCanceled->errorcode());
        INFO_LOG("CanceledCallBack error = " << pCanceled->error());
        if(pOrder)
        {
            INFO_LOG("CanceledCallBack order_id = " << pOrder->order_id());
            INFO_LOG("CanceledCallBack type = " << pOrder->type());
            INFO_LOG("CanceledCallBack from_address = " << pOrder->from_address());
            INFO_LOG("CanceledCallBack to_address = " << pOrder->to_address());
            INFO_LOG("CanceledCallBack description = " << pOrder->description());
            INFO_LOG("CanceledCallBack amount = " << pOrder->amount());
            INFO_LOG("CanceledCallBack fee = " << pOrder->fee());
            INFO_LOG("CanceledCallBack order_date = " << pOrder->order_date());
            INFO_LOG("CanceledCallBack order_time = " << pOrder->order_time());
            INFO_LOG("CanceledCallBack status = " << pOrder->status());
        }
    }

    SerializeToArray(cb_message);
    pCanceled->release_order();
    pOrder = NULL;
    cb_message.release_canceled_info();
    pCanceled = NULL;
}

void CallBackManager::RejectedCallBack(FrontEngine::CallBackMessage &cb_message)
{
    INFO_LOG("RejectedCallBack client_id = " << cb_message.client_id());
    FrontEngine::RejectedCallBack* pRejected = cb_message.mutable_rejected_info();
    FrontEngine::Order* pOrder = pRejected->mutable_order();

    if(pRejected)
    {
        INFO_LOG("RejectedCallBack request_id = " << pRejected->request_id());
        INFO_LOG("RejectedCallBack errorCode = " << pRejected->errorcode());
        INFO_LOG("RejectedCallBack error = " << pRejected->error());
        if(pOrder)
        {
            INFO_LOG("RejectedCallBack order_id = " << pOrder->order_id());
            INFO_LOG("RejectedCallBack type = " << pOrder->type());
            INFO_LOG("RejectedCallBack from_address = " << pOrder->from_address());
            INFO_LOG("RejectedCallBack to_address = " << pOrder->to_address());
            INFO_LOG("RejectedCallBack description = " << pOrder->description());
            INFO_LOG("RejectedCallBack amount = " << pOrder->amount());
            INFO_LOG("RejectedCallBack fee = " << pOrder->fee());
            INFO_LOG("RejectedCallBack order_date = " << pOrder->order_date());
            INFO_LOG("RejectedCallBack order_time = " << pOrder->order_time());
            INFO_LOG("RejectedCallBack status = " << pOrder->status());
        }  
    }

    SerializeToArray(cb_message);
    pRejected->release_order();
    pOrder = NULL;
    cb_message.release_traded_info();
    pRejected = NULL;
}

void CallBackManager::SerializeToArray(const FrontEngine::CallBackMessage &cb_message)
{
    char message[MESSAGE_BODY_SIZE] = {'\0'};
    cb_message.SerializeToArray(message, MESSAGE_BODY_SIZE);
    DataManager::GetInstance()->CallBackQueuePush(cb_message.client_id(), message);
}

bool CallBackManager::SetMd5(char *message)
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

bool CallBackManager::SetMd5(FrontEngine::CallBackMessage &cb_message)
{
    char message[MESSAGE_BODY_SIZE] = {'\0'};
    cb_message.SerializeToArray(message, MESSAGE_BODY_SIZE);

    MD5 md5(message, MESSAGE_BODY_SIZE);
    cb_message.set_md5(md5.toString());
}

}