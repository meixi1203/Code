/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef CALLBACK_MANAGER_H
#define CALLBACK_MANAGER_H

#include "wallet_front_base.h"

namespace walletfront {

class CallBackManager
{
    public:
        CallBackManager();
        ~CallBackManager();

    public:
        void ZmqSub();

    private:
        void OnResponse(const char *message);

    private:
        std::string m_ip;
        std::string m_port;
};

}

#endif