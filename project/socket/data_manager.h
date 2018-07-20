/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "wallet_front_base.h"
#include "blockingconcurrentqueue.h"
#include <map>

namespace walletfront {

class DataManager
{
    public:
        static DataManager* GetInstance();
        // RecvQueue IF
        bool RecvQueuePush(const int &socket, const char buff[]);
        bool RecvQueueBack(int &socket, char buff[]);
        int RecvQueueSize() { return m_recv_queue.size_approx(); }
        // CallBackQueue IF
        bool CallBackQueuePush(const std::string &sdk_id, const char buff[]);
        bool CallBackQueueBack(const std::string &sdk_id, char buff[]);
        int  CallBackQueueSize(const std::string &sdk_id) { return m_callback_queue[sdk_id].size_approx(); }
        bool CallBackQueueEmpty(const int &socket);
        // callBack for check App id
        bool CBQueuePush(const int &socket, const char buff[]);
        bool CBQueueBack(const int &socket, char buff[]);
        int CBQueueSize(const int &socket) { return m_cb_for_checkappid[socket].size_approx(); }

        // ClientInfo IF
        bool ClientInfoMapInsert(const int &socket, const std::string &sdk_id, const int &port, const std::string &ip);
        bool ClientInfoMapUpdate(const int &socket, const std::string &sdk_id);
        bool ClientInfoMapErase(const std::string &sdk_id);
        bool ClientInfoMapErase(const int &socket);
        std::string ClientInfoMapFind(const int &socket);

    private:
        DataManager();
        ~DataManager();
        DataManager(const DataManager&);
        DataManager& operator=(const DataManager&);

    private:
        moodycamel::BlockingConcurrentQueue<struct RecvBuff> m_recv_queue;
        std::map<std::string, moodycamel::BlockingConcurrentQueue<std::string>> m_callback_queue;
        std::map<int, moodycamel::BlockingConcurrentQueue<std::string>> m_cb_for_checkappid; // only for CheckAppId
        std::map<int, struct ClientInfo>m_client_info;
};
}

#endif