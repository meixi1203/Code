/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "data_manager.h"
#include <string.h>

#define MESSAGE_BODY_SIZE 4096
namespace walletfront {

DataManager* DataManager::GetInstance()
{
    static DataManager * pDateMmannger = new DataManager();

    if(pDateMmannger == NULL)
    {
        //INFO_LOG("GetInstance serror msg_queue == NULL ");
    }
    return pDateMmannger;
}

DataManager::DataManager()
{
    m_sdk_id = "sdk_id";
}

DataManager::~DataManager()
{

}

bool DataManager::ReqQueuePush(const char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_req_queue.enqueue(temp_str);
    return true;
}

bool DataManager::ReqQueueBack(char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_req_queue.wait_dequeue(temp_str);
    memcpy(buff, temp_str.c_str(), MESSAGE_BODY_SIZE);
    return true;
}

bool DataManager::CallBackQueuePush( const char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_rely_queue.enqueue(temp_str);
    return true;
}

bool DataManager::CallBackQueueBack( char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_rely_queue.wait_dequeue(temp_str);
    memcpy(buff, temp_str.c_str(), MESSAGE_BODY_SIZE);
    return true;
}


bool DataManager::SetSdkId(const std::string &sdk_id)
{
    m_sdk_id = sdk_id;
    return true;
}

std::string DataManager::GetSdkId()
{
    return m_sdk_id;
}
}