/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef WALLET_FRONT_SOCKET_H
#define WALLET_FRONT_SOCKET_H

#include "wallet_front_base.h"
#include <sys/epoll.h>

#define EPOLL_EVENTS_NUM 1024

namespace walletfront {

class Socket
{
    public:
        Socket();
        ~Socket();

    public:
        void ServiceStart();
        void SocketInit();
        void EpollRun();
        int    ReadData(int socket, char recv_buf[]);
        int    WriteData(int socket, const char* send_buf);
        void SendMessage();

    private:
        void ServiceBind();
        void ServiceListen();
        void EpollPrepare();
        void SetNBlock(int sock);
        void SendMsg(int socket);
        int    ServiceAccept();
        bool SetEpollEvent(int op, int events, int socket);

    private:
            Socket(const Socket&);
            Socket& operator=(const Socket&);

    private:
        int m_port;
        std::string m_ip;
        int m_service_sock;
        int m_epfd;
        struct epoll_event m_events[EPOLL_EVENTS_NUM];
};

}
#endif