/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "blockingconcurrentqueue.h"
#include <map>

namespace walletfront {

class DataManager
{
    public:
        static DataManager* GetInstance();
        // RecvQueue IF
        bool ReqQueuePush(const char buff[]);
        bool ReqQueueBack(char buff[]);
        int ReqQueueSize() { return m_req_queue.size_approx(); }

        // CallBackQueue IF
        bool CallBackQueuePush(const char buff[]);
        bool CallBackQueueBack(char buff[]);
        int CallBackQueueSize() { return m_rely_queue.size_approx(); }

        bool SetSdkId(const std::string &sdk_id);
        std::string GetSdkId();

    private:
        DataManager();
        ~DataManager();
        DataManager(const DataManager&);
        DataManager& operator=(const DataManager&);

    private:
        moodycamel::BlockingConcurrentQueue<std::string> m_req_queue;
        moodycamel::BlockingConcurrentQueue<std::string>  m_rely_queue;
        std::string m_sdk_id;
};
}

#endif