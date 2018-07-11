/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef SEND_MANAGER_H
#define SEND_MANAGER_H

#include "front_engine.pb.h"
#include <vector>
#include <functional>

namespace walletfront {

class SendManager
{
    public:
        SendManager();
        ~SendManager();

        void run();
        void SendManagerInit();
        void CheckAppid_No();
        void CheckAppid_Ok();
        void ReqHeartBeat();
        void ReqCreatAccount();
        std::string ChangeIntToStr();
        
    private:
        SendManager(const SendManager&);
        SendManager& operator=(const SendManager&);
        std::vector<std::function<void()>> m_reqfun_vec;

};
}

#endif //SEND_MANAGER_H