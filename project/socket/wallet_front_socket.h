/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef WALLET_FRONT_SOCKET_H
#define WALLET_FRONT_SOCKET_H

#include "wallet_front_base.h"
#include <sys/epoll.h>
#include <vector>


namespace walletfront {

class Socket
{
    public:
        Socket();
        ~Socket();

    public:
        void ServiceStart();
        void ServiceEpollRun();
        int ReadData(int socket, char recv_buf[]);
        int WriteData(int socket, const char* send_buf);

    private:
        void ServiceBind();
        void ServiceListen();
        int ServiceAccept();
        bool SetEpollEvent(int op, int events, int socket);
        void SetNBlock(int sock);
        void ServiceEpollPrepare();

    private:
        int m_port;
        std::string m_ip;
        int m_service_sock;
        int m_epfd;
        struct epoll_event m_events[20];
};

}
#endif