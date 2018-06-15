/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef WALLET_GRONT_BASE_H
#define WALLET_GRONT_BASE_H

#include <iostream>

#define MESSAGE_BODY_SIZE 4096

namespace walletfront {

struct ClientInfo
{
    int port;
    std::string ip;
    int count;  // for Heart-Beat
    bool status; // client status
};

}
#endif