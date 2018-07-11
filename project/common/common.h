/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef COMMON_H
#define COMMON_H

#include "wallet_front_base.h"

namespace walletfront {

class Common
{
    public:
        void InitServerFromConf();
        std::string GetWalletServiceIP();
        std::string GetWalletServicePort();
        std::string GetPushService();
        std::string GetPubService();

        static Common* GetInstance();

    private:
       struct ConfigFile  m_conf_file;

    private:
        Common();
        ~Common();
        Common(const Common&);
        Common& operator=(const Common&);
};
}

#endif //COMMON_H