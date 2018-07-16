/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "callback_manager.h"
#include "data_manager.h"
#include "md5.h"
#include <thread>
#include <unistd.h>
#include <iostream>

#define MESSAGE_BODY_SIZE 4096
#define STRING_EMPTY ""

namespace walletfront {

CallBackManager::CallBackManager()
{

}

CallBackManager::~CallBackManager()
{

}

void CallBackManager::CallBackManagerInit()
{
    std::thread recv_ths(&CallBackManager::run, this);
    recv_ths.detach();
}

void CallBackManager::run()
{
    while(1)
    {
        
        if(DataManager::GetInstance()->CallBackQueueSize() > 0)
        {
            char buffer[MESSAGE_BODY_SIZE] = "";
            DataManager::GetInstance()->CallBackQueueBack(buffer);
            if(CheckMD5(buffer))
            {
                OnResponse(buffer);
            }
        }
        else
        {
            sleep(0.5);
        }
    }
}

void CallBackManager::OnResponse(const char * msg)
{
    if(msg == NULL)
    {
        return;
    }

    FrontEngine::CallBackMessage cb_message;
    cb_message.ParseFromArray(msg, MESSAGE_BODY_SIZE);
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " message type = " << cb_message.type() << std::endl;
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " message client_id = " << cb_message.client_id() << std::endl;
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " message request_id = " << cb_message.request_id() << std::endl;
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " message front_id = " << cb_message.front_id() << std::endl;

    if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_CheckAppIdCallBack)
    {
        CheckAppIdCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_HeartBeatCallBack)
    {
        HeatBeatCallBack(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_CreateAccountCallBack)
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
        //
    }
}

bool CallBackManager::CheckMD5(char * msg)
{
    bool ret = false;
    char buffer[MESSAGE_BODY_SIZE] = {'\0'};

    FrontEngine::CallBackMessage cb_message;
    cb_message.ParseFromArray(msg, MESSAGE_BODY_SIZE);

    std::string recv_md5 = cb_message.md5();
    cb_message.set_md5(STRING_EMPTY);
    
    cb_message.SerializeToArray(buffer, MESSAGE_BODY_SIZE );
    MD5 md5(buffer, MESSAGE_BODY_SIZE);
    std::string current_md5 = md5.toString();

    if(recv_md5 == current_md5 )
    {
        ret = true;
    }
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__  << " line: " << __LINE__ << " CheckMD5  ret  = " << ret << std::endl;
    return ret;
    
}

void CallBackManager::HeatBeatCallBack(FrontEngine::CallBackMessage &cb_message)
{
    FrontEngine::HeartBeatCallBack* pHeartBeatCb = cb_message.mutable_heart_beat();
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__  << " line: " << __LINE__ << " HeatBeatCb  type =  "  <<  cb_message.type() << "  data  = " << pHeartBeatCb->data() << std::endl;
    cb_message.release_heart_beat();
    pHeartBeatCb = NULL;
}

void CallBackManager::CheckAppIdCallBack(FrontEngine::CallBackMessage &cb_message)
{
    FrontEngine::CheckAppIdCallBack* pCheckAppId = cb_message.mutable_app_id();
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CheckAppidCb  type =  "  << cb_message.type() << "  sdk id  = " <<  pCheckAppId->sdk_id() << std::endl;
    if(!pCheckAppId->sdk_id().empty())
    {
        DataManager::GetInstance()->SetSdkId(pCheckAppId->sdk_id());
    }
    
    cb_message.release_app_id();
    pCheckAppId = NULL;
}

void CallBackManager::CreatAccountCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::CreateAccountCallBack* pCreatAccount = cb_message.mutable_account();
    FrontEngine::UserInfo* pUserInfo = pCreatAccount->mutable_account();

    if(pCreatAccount)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack request_id = " << pCreatAccount->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack errorCode = " << pCreatAccount->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack error = " << pCreatAccount->error() << std::endl;
        if(pUserInfo)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack type = " << pUserInfo->type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack username = " << pUserInfo->username() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack password = " << pUserInfo->password() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CreatAccountCallBack address = " << pUserInfo->address() << std::endl;
        }
    }

    pCreatAccount->release_account();
    pUserInfo = NULL;
    cb_message.release_account();
    pCreatAccount = NULL;
}

void CallBackManager::QueryBalanceCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::QueryBalanceCallBack* pQueryBalance = cb_message.mutable_balance();
    FrontEngine::CoinCapital* pCoinCapital = pQueryBalance->mutable_capital();

    if(pQueryBalance)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack request_id = " << pQueryBalance->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack errorCode = " << pQueryBalance->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack error = " << pQueryBalance->error() << std::endl;
        if(pCoinCapital)
        {
           std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack address = " << pCoinCapital->address() << std::endl;
           std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack type = " << pCoinCapital->type() << std::endl;
           std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack amount = " << pCoinCapital->amount() << std::endl;
           std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryBalanceCallBack frozen_amount = " << pCoinCapital->frozen_amount() << std::endl;
        }
    }

    pQueryBalance->release_capital();
    pCoinCapital = NULL;
    cb_message.release_account();
    pQueryBalance = NULL;
}

void CallBackManager::QueryFeeCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::QueryFeeCallBack* pQueryFee = cb_message.mutable_fee();

    if(pQueryFee)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack request_id = " << pQueryFee->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack coin_type = " << pQueryFee->coin_type() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack fee = " << pQueryFee->fee() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack errorCode = " << pQueryFee->errorcode() << std::endl; 
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryFeeCallBack error = " << pQueryFee->error() << std::endl;
    }

    cb_message.release_fee();
    pQueryFee = NULL;
}

void CallBackManager::QueryOrderCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::QueryOrderCallBack* pQueryOrder = cb_message.mutable_order();
    FrontEngine::Order* pOrder = pQueryOrder->mutable_order();

    if(pQueryOrder)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack request_id = " << pQueryOrder->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack request_id = " << pQueryOrder->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack errorCode = " << pQueryOrder->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack error = " << pQueryOrder->error() << std::endl;
        if(pOrder)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack order_id = " << pOrder->order_id() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack type = " << pOrder->type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack from_address = " << pOrder->from_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack to_address = " << pOrder->to_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack description = " << pOrder->description() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack amount = " << pOrder->amount() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack fee = " << pOrder->fee() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack order_date = " << pOrder->order_date() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack order_time = " << pOrder->order_time() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrderCallBack status = " << pOrder->status() << std::endl;
        }
    }

    pQueryOrder->release_order();
    pOrder = NULL;
    cb_message.release_order();
    pQueryOrder = NULL;
}

void CallBackManager::QueryOrdersCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::QueryOrdersCallBack* pQueryOrders = cb_message.mutable_orders();

    if(pQueryOrders)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack request_id = " << pQueryOrders->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack errorCode = " << pQueryOrders->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack error = " << pQueryOrders->error() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack length = " << pQueryOrders->length() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack length = " << pQueryOrders->length() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack length = " << pQueryOrders->length() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack order size = " << pQueryOrders->order_size() << std::endl;
        for(int index = 0; index < pQueryOrders->order_size(); index++)
        {
            FrontEngine::Order order = pQueryOrders->order(index);
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack order_id = " << order.order_id() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack type = " << order.type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack from_address = " << order.from_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack to_address = " << order.to_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack description = " << order.description() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack amount = " << order.amount() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack fee = " << order.fee() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack order_date = " << order.order_date() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack order_time = " << order.order_time() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " QueryOrdersCallBack status = " << order.status() << std::endl;
        }
    }

    cb_message.release_orders();
    pQueryOrders = NULL;
}

void CallBackManager::TradedCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::TradedCallBack* pTraded = cb_message.mutable_traded_info();
    FrontEngine::Order* pOrder = pTraded->mutable_order();

    if(pTraded)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack request_id = " << pTraded->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack errorCode = " << pTraded->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack error = " << pTraded->error() << std::endl;
        if(pOrder)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack order_id = " << pOrder->order_id() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack type = " << pOrder->type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack from_address = " << pOrder->from_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack to_address = " << pOrder->to_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack description = " << pOrder->description() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack amount = " << pOrder->amount() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack fee = " << pOrder->fee() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack order_date = " << pOrder->order_date() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack order_time = " << pOrder->order_time() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " TradedCallBack status = " << pOrder->status() << std::endl;
        }
    }

    pTraded->release_order();
    pOrder = NULL;
    cb_message.release_traded_info();
    pTraded = NULL;
}

void CallBackManager::CanceledCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ <<"CanceledCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::CanceledCallBack* pCanceled = cb_message.mutable_canceled_info();
    FrontEngine::Order* pOrder = pCanceled->mutable_order();

    if(pCanceled)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack request_id = " << pCanceled->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack errorCode = " << pCanceled->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack error = " << pCanceled->error() << std::endl;
        if(pOrder)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack order_id = " << pOrder->order_id() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack type = " << pOrder->type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack from_address = " << pOrder->from_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack to_address = " << pOrder->to_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack description = " << pOrder->description() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack amount = " << pOrder->amount() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack fee = " << pOrder->fee() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack order_date = " << pOrder->order_date() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack order_time = " << pOrder->order_time() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " CanceledCallBack status = " << pOrder->status() << std::endl;
        }
    }

    pCanceled->release_order();
    pOrder = NULL;
    cb_message.release_canceled_info();
    pCanceled = NULL;
}

void CallBackManager::RejectedCallBack(FrontEngine::CallBackMessage &cb_message)
{
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack client_id = " << cb_message.client_id() << std::endl;
    FrontEngine::RejectedCallBack* pRejected = cb_message.mutable_rejected_info();
    FrontEngine::Order* pOrder = pRejected->mutable_order();

    if(pRejected)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack request_id = " << pRejected->request_id() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack errorCode = " << pRejected->errorcode() << std::endl;
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack error = " << pRejected->error() << std::endl;
        if(pOrder)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack order_id = " << pOrder->order_id() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack type = " << pOrder->type() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack from_address = " << pOrder->from_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack to_address = " << pOrder->to_address() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack description = " << pOrder->description() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack amount = " << pOrder->amount() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack fee = " << pOrder->fee() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack order_date = " << pOrder->order_date() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack order_time = " << pOrder->order_time() << std::endl;
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " RejectedCallBack status = " << pOrder->status() << std::endl;
        }  
    }

    pRejected->release_order();
    pOrder = NULL;
    cb_message.release_traded_info();
    pRejected = NULL;
}

}
