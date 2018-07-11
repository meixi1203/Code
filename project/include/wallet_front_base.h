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
    std::string sdk_id;
};

struct ConfigFile
{
    std::string wallet_server_ip;
    std::string wallet_server_port;
    std::string push_server;
    std::string pull_server;
};

struct RecvBuff
{
    int socket;
    std::string msg;
};

}
#endif