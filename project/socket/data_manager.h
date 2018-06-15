/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "wallet_front_base.h"
#include <queue>
#include <map>


namespace walletfront {

class DataManager
{
    public:
        static DataManager* GetInstance();

        bool RecvQueuePush(const char buff[]);
        bool RecvQueueBack(char buff[]);
        int RecvQueueSize() { return m_recv_queue.size(); }
        bool RecvQueueEmpty() { return m_recv_queue.empty(); }

        bool CallBackQueuePush(const char buff[]);
        bool CallBackQueueBack(char buff[]);
        int CallBackQueueSize() { return m_callback_queue.size(); }
        bool CallBackQueueEmpty() { return m_callback_queue.empty(); }

        bool ClientInfoMapInsert(int sdk_id, struct ClientInfo &client_info);
        bool ClientInfoMapEmpty() { return m_client_info.empty(); }
        int ClientInfoMapSize() { return m_client_info.size(); }
        int GetClientInfoMapCount(int index);
        int GetClientInfoMapSdkId(int index);
        bool SetClientInfoMapCount(int index);
        bool ClearClientInfoMapCount(int sdk_id);

    public:
        DataManager();
        ~DataManager();

    private:
        std::queue<char *> m_recv_queue;
        std::queue<char *> m_callback_queue;
        std::map<int, struct ClientInfo>m_client_info;
};
}

#endif