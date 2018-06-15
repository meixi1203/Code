/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

# include "data_manager.h"
#include "log.h"
#include <string.h>

namespace walletfront {

DataManager* DataManager::GetInstance()
{
    static DataManager * date_mannger = new DataManager();

    if(date_mannger == NULL)
    {
        TRACE_LOG("GetInstance serror msg_queue == NULL ");
    }
    return date_mannger;
}

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

bool DataManager::RecvQueuePush(const char buff[])
{
    char temp_buf[MESSAGE_BODY_SIZE] =  { '\0' };
    memcpy(temp_buf, buff, MESSAGE_BODY_SIZE);
    m_recv_queue.push(temp_buf);

    return true;
}

bool DataManager::RecvQueueBack(char buff[])
{
    memcpy(buff, m_recv_queue.back(), MESSAGE_BODY_SIZE);
    m_recv_queue.pop();
    return true;
}

bool DataManager::CallBackQueuePush(const char buff[])
{
    char temp_buf[MESSAGE_BODY_SIZE] =  { '\0' };
    memcpy(temp_buf, buff, MESSAGE_BODY_SIZE);
    m_callback_queue.push(temp_buf);

    return true;
}

bool DataManager::CallBackQueueBack(char buff[])
{
    memcpy(buff, m_callback_queue.back(), MESSAGE_BODY_SIZE);
    m_callback_queue.pop();

    return true;
}

bool DataManager::ClientInfoMapInsert(int sdk_id, struct ClientInfo &client_info)
{
    bool ret = false;
    std::pair<std::map<int, struct ClientInfo>::iterator, bool> insert_pair;

    insert_pair = m_client_info.insert(std::pair<int, struct ClientInfo>(sdk_id, client_info));

    if(insert_pair.second == true)
    {
        ret = true;
    }
    return ret;
}

int DataManager::GetClientInfoMapCount(int index)
{
    int count = -1;

    if (index  >= 0 )
    {
        int sdk_id = GetClientInfoMapSdkId(index);
        count = m_client_info[sdk_id].count;
    }

    return count;
}

int DataManager::GetClientInfoMapSdkId(int index)
{
    int sdk_id = -1;

    std::map<int, struct ClientInfo>::iterator iter = m_client_info.begin();
    while(index--  >= 0 )
    {
       iter++;
    }

    if(iter != m_client_info.end())
    {
        sdk_id = iter->first;
    }

    return sdk_id;
}

bool DataManager::SetClientInfoMapCount(int index)
{
    bool ret = false;

    if (index  >= 0 )
    {
        int sdk_id = GetClientInfoMapSdkId(index);
        m_client_info[sdk_id].count++;
        ret = true;
    }

    return ret;
}

bool DataManager::ClearClientInfoMapCount(int sdk_id)
{
    bool ret = false;

    if(sdk_id >0 )
    {
        m_client_info[sdk_id].count = 0;
        ret = true;
    }

    return ret;

}

}