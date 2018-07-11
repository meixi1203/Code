/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include "front_engine.pb.h"
#include <iostream>
#include <map>
#include <mutex>

namespace walletfront {
class CallBackManager;
class RequestManager
{
    public:
        RequestManager();
        ~RequestManager();

    public:
        void Run();
        void RequestManagerInit();

    private:
        void OnRequest(const int &socket, const char *message);
        void HeartBeatHandler();

    private:
        void SendManage(const char *message);
        void CheckAppId(const int &socket, FrontEngine::RequestMessage &req_message);
        void RequestHeartBeat(FrontEngine::RequestMessage &req_message);
        void RequestCreatAccount(FrontEngine::RequestMessage &req_message);
        void RequestQueryBalance(FrontEngine::RequestMessage &req_message);
        void QueryFeeRequest(FrontEngine::RequestMessage &req_message);
        void RequestQueryOrder(FrontEngine::RequestMessage &req_message);
        void RequestQueryOrders(FrontEngine::RequestMessage &req_message);
        void RequestSendCoin(FrontEngine::RequestMessage &req_message);
        void RequestRecieveCoin(FrontEngine::RequestMessage &req_message);
        bool CheckMd5(char *message);
        bool SetMd5(char *message);
        bool SetHeartBeatCount(const std::string &sdk_id);

        RequestManager(const RequestManager&);
        RequestManager& operator=(const RequestManager&);

    private:
        std::map<std::string, int> m_heat_count;
        CallBackManager*            m_pCallBack;

        std::mutex m_heat_mtux;
};

}
#endif //REQUEST_MANAGER_H