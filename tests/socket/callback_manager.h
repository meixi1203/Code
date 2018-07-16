/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include "front_engine.pb.h"

namespace walletfront {

class CallBackManager
{
    public:
        CallBackManager();
        ~CallBackManager();
        void CallBackManagerInit();

        void run();
        void OnResponse(const char * msg);
        bool CheckMD5(char * msg);
        void HeatBeatCallBack(FrontEngine::CallBackMessage &cb_message);
        void CheckAppIdCallBack(FrontEngine::CallBackMessage &cb_message);
        void CreatAccountCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryBalanceCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryFeeCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryOrderCallBack(FrontEngine::CallBackMessage &cb_message);
        void QueryOrdersCallBack(FrontEngine::CallBackMessage &cb_message);
        void TradedCallBack(FrontEngine::CallBackMessage &cb_message);
        void CanceledCallBack(FrontEngine::CallBackMessage &cb_message);
        void RejectedCallBack(FrontEngine::CallBackMessage &cb_message);
        

    private:
        CallBackManager(const CallBackManager&);
        CallBackManager& operator=(const CallBackManager&);
};
}

#endif //CALLBACK_MANAGER_H