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
        void HeatBeatCb(FrontEngine::CallBackMessage &cb_message);
        void CheckAppidCb(FrontEngine::CallBackMessage &cb_message);
        

    private:
        CallBackManager(const CallBackManager&);
        CallBackManager& operator=(const CallBackManager&);
};
}

#endif //CALLBACK_MANAGER_H