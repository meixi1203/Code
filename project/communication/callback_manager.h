/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include "wallet_front_base.h"
#include "front_engine.pb.h"

namespace walletfront {

class CallBackManager
{
    public:
        CallBackManager();
        ~CallBackManager();

    public:
        void ZmqSub();
        void CallBackManagerInit();
        bool SetMd5(char *message);
        bool SetMd5(FrontEngine::CallBackMessage &cb_message);
        void SerializeToArray(const FrontEngine::CallBackMessage &cb_message);

    private:
        void OnResponse(const char *message);
        void CreatAccountCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryBalanceCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryFeeCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryOrderCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryOrdersCallBack(FrontEngine::CallBackMessage &cb_message);
        void TradedCallBack(FrontEngine::CallBackMessage &cb_message);
        void CanceledCallBack(FrontEngine::CallBackMessage &cb_message);
        void RejectedCallBack(FrontEngine::CallBackMessage &cb_message);

        CallBackManager(const CallBackManager&);
        CallBackManager& operator=(const CallBackManager&);

};

}

#endif //CALLBACK_MANAGER_H