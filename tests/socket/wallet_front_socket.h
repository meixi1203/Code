/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef WALLET_FRONT_SOCKET_H
#define WALLET_FRONT_SOCKET_H

#include <sys/epoll.h>
#include <vector>
#include <iostream>

namespace walletfront {

#define MESSAGE_BODY_SIZE 4096

class SocketClient
{
    public:
        SocketClient();
        ~SocketClient();

    public:
        void ClientConnect();
        int read(char szBuffer[]);
        void write(const char* str);
        void SocketClientInit();
        void SendRun();
        void RecvRun();

    private:
        int m_port;
        std::string m_ip;
        int m_client_sock;
};
}
#endif