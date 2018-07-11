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
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " DataManager::GetInstance()->CallBackQueueSize() = " << DataManager::GetInstance()->CallBackQueueSize() << std::endl;
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
        CheckAppidCb(cb_message);
    }
    else if(cb_message.type() == FrontEngine::enums_CallBackType::enums_CallBackType_HeartBeatCallBack)
    {
        HeatBeatCb(cb_message);
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

    if(recv_md5 ==current_md5 )
    {
        ret = true;
    }
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__  << " line: " << __LINE__ << " CheckMD5  ret  = " << ret << std::endl;
    return ret;
    
}

void CallBackManager::HeatBeatCb(FrontEngine::CallBackMessage &cb_message)
{
    FrontEngine::HeartBeatCallBack* pHeartBeatCb = cb_message.mutable_heart_beat();
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__  << " line: " << __LINE__ << " HeatBeatCb  type =  "  <<  cb_message.type() << "  data  = " << pHeartBeatCb->data() << std::endl;
    cb_message.release_heart_beat();
    pHeartBeatCb = NULL;
}

void CallBackManager::CheckAppidCb(FrontEngine::CallBackMessage &cb_message)
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
}
