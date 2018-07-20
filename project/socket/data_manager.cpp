/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "data_manager.h"
#include "log.h"
#include <string.h>

namespace walletfront {

DataManager* DataManager::GetInstance()
{
    static DataManager * pDateMmannger = new DataManager();

    if(pDateMmannger == NULL)
    {
        ERROR_LOG("GetInstance serror msg_queue == NULL ");
    }
    return pDateMmannger;
}

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

bool DataManager::RecvQueuePush(const int &socket, const char buff[])
{
    struct RecvBuff temp_buff;
    temp_buff.socket = socket;
    std::string temp_str(buff, MESSAGE_BODY_SIZE);
    temp_buff.msg = temp_str;
    m_recv_queue.enqueue(temp_buff);
    return true;
}

bool DataManager::RecvQueueBack(int &socket, char buff[])
{
    struct RecvBuff temp_buff;
    m_recv_queue.wait_dequeue(temp_buff);
    socket = temp_buff.socket;
    memcpy(buff, temp_buff.msg.c_str(), MESSAGE_BODY_SIZE);
    return true;
}

bool DataManager::CallBackQueuePush(const std::string &sdk_id, const char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_callback_queue[sdk_id].enqueue(temp_str);
    return true;
}

bool DataManager::CallBackQueueBack(const std::string &sdk_id, char buff[])
{
    std::string temp_str;
    m_callback_queue[sdk_id].wait_dequeue(temp_str);
    memcpy(buff, temp_str.c_str(), MESSAGE_BODY_SIZE);
    return true;
}

bool DataManager::CallBackQueueEmpty(const int &socket)
{
    bool bRet = true;
    std::string sdk_id = ClientInfoMapFind(socket);

    if(!sdk_id.empty())
    {
        if(CallBackQueueSize(sdk_id) > 0)
        {
            bRet = false;
        }
    }
    return bRet;
}

bool DataManager::CBQueuePush(const int &socket, const char buff[])
{
    std::string temp_str(buff,MESSAGE_BODY_SIZE);
    m_cb_for_checkappid[socket].enqueue(temp_str);
    return true;
}

bool DataManager::CBQueueBack(const int &socket, char buff[])
{
    std::string temp_str;
    m_cb_for_checkappid[socket].wait_dequeue(temp_str);
    memcpy(buff, temp_str.c_str(), MESSAGE_BODY_SIZE);
    return true;
}

bool DataManager::ClientInfoMapInsert(const int &socket, const std::string &sdk_id, const int &port, const std::string &ip)
{
    struct ClientInfo client_info;
    client_info.ip = ip;
    client_info.port = port;
    client_info.sdk_id = sdk_id;
    m_client_info[socket] = client_info;

    INFO_LOG(" m_client_info   socket = " << socket);
    INFO_LOG(" m_client_info   sdk_id = " << client_info.sdk_id);
    INFO_LOG(" m_client_info   ip = " << client_info.ip);
    INFO_LOG(" m_client_info   port = " << client_info.port);
    INFO_LOG(" m_client_info   current socket size = " << m_client_info.size());
    return true;
}

bool DataManager::ClientInfoMapUpdate(const int &socket, const std::string &sdk_id)
{
    m_client_info[socket].sdk_id = sdk_id;
    return true;
}

bool DataManager::ClientInfoMapErase(const std::string &sdk_id)
{
    bool bRet = false;
    std::map<int, struct ClientInfo>::iterator iter;

    for(iter = m_client_info.begin(); iter != m_client_info.end(); iter++)
    {
        if(iter->second.sdk_id == sdk_id)
        {
            INFO_LOG(" m_client_info   socket = " << iter->first);
            INFO_LOG(" m_client_info   sdk_id = " << iter->second.sdk_id);
            INFO_LOG(" m_client_info   ip = " << iter->second.ip);
            INFO_LOG(" m_client_info   port = " << iter->second.port);
            m_client_info.erase(iter);
            bRet = true;
            break;
        }
        INFO_LOG(" m_client_info   current socket size = " << m_client_info.size());
    }
    return bRet;
}

bool DataManager::ClientInfoMapErase(const int &socket)
{
    bool bRet = false;
    std::map<int, struct ClientInfo>::iterator iter;
    iter = m_client_info.find(socket);

    if(iter != m_client_info.end())
    {
        INFO_LOG(" m_client_info   socket = " << iter->first);
        INFO_LOG(" m_client_info   sdk_id = " << iter->second.sdk_id);
        INFO_LOG(" m_client_info   ip = " << iter->second.ip);
        INFO_LOG(" m_client_info   port = " << iter->second.port);
        m_client_info.erase(iter);
        bRet = true;
    }
    INFO_LOG(" m_client_info   current socket size = " << m_client_info.size());
    return bRet;
}

std::string DataManager::ClientInfoMapFind(const int &socket)
{
    std::string ret("");
    std::map<int, struct ClientInfo>::iterator iter;
    iter = m_client_info.find(socket);

    if(iter != m_client_info.end())
    {
       ret = iter->second.sdk_id;
    }
    //INFO_LOG(" find   sdk_id = " << ret);

    return ret;
}

}