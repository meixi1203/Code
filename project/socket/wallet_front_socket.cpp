/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "wallet_front_socket.h"
#include "data_manager.h"
#include "common.h"
#include "log.h"
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace walletfront {

Socket::Socket()
{
    m_port = 8080;
    m_ip = "";
    m_epfd = 0;
    m_service_sock = 0;

}

Socket::~Socket()
{
    m_port = 8080;
    m_ip = "";
    close(m_epfd);
    close(m_service_sock);
}

void Socket::SocketInit()
{
    this->m_ip = walletfront::Common::GetInstance()->GetWalletServiceIP();
    this->m_port = atoi(walletfront::Common::GetInstance()->GetWalletServicePort().c_str());

    //reset m_events
    memset(&m_events, 0, sizeof(struct epoll_event)*EPOLL_EVENTS_NUM);
    ServiceStart();
}

void Socket::ServiceStart()
{
    ServiceBind();
    EpollPrepare();
}

void Socket::ServiceBind()
{
    if((m_service_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        ERROR_LOG("ServiceBind  socket error");
    }

    sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family = PF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(this->m_ip.c_str());
    sock_addr.sin_port = htons(this->m_port);

    if (bind(m_service_sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr)) < 0)
    {
        ERROR_LOG("ServiceBind  bind error");
    }

    SetNBlock(m_service_sock);
    ServiceListen();
}

void Socket::ServiceListen()
{
    if((listen(m_service_sock, 20) < 0))
    {
        ERROR_LOG("listen error");
    }
}

void Socket::SetNBlock(int sock)
{
    int opts = 0;

    opts = fcntl(sock, F_GETFL); //获取文件的flags值。
    if(opts < 0)
    {
        ERROR_LOG("fcntl l(sock,GETFL) error");
    }

    opts = opts | O_NONBLOCK; //设置成非阻塞模式
    if(fcntl(sock ,F_SETFL, opts) < 0 )
    {
        ERROR_LOG("fcntl(sock,SETFL,opts) error");
    }
}

void Socket::EpollPrepare()
{
    if((m_epfd = epoll_create(EPOLL_EVENTS_NUM)) == -1)
    {
        ERROR_LOG("epoll_create error");
    }

    if(!SetEpollEvent(EPOLL_CTL_ADD, EPOLLIN, m_service_sock))
    {
        ERROR_LOG("SetEpollEvent error");
    }
}

void Socket::EpollRun()
{
    int nfds = epoll_wait(m_epfd, m_events, EPOLL_EVENTS_NUM, 500);

    if(nfds == -1)
    {
        ERROR_LOG("epoll_wait error");
    }

    for(int i = 0; i < nfds; ++i)
    {
        if(m_events[i].data.fd == m_service_sock)
        {
            ServiceAccept();
        }

        else if (m_events[i].events & EPOLLIN)
        {
            char recv_msg[MESSAGE_BODY_SIZE] = {'\0'};
            int len = ReadData(m_events[i].data.fd, recv_msg);

            if(len == MESSAGE_BODY_SIZE && recv_msg != NULL)
            {
                DataManager::GetInstance()->RecvQueuePush(m_events[i].data.fd, recv_msg);
            }
        }
        else if(m_events[i].events & EPOLLOUT)
        {
            SendMsg(m_events[i].data.fd);
        }
    }
}

int Socket::ReadData(int socket, char recv_buf[])
{
    char buf[MESSAGE_BODY_SIZE] = { '\0' };
    int len = 0;
    int ret = 0;

    while(len < MESSAGE_BODY_SIZE)
    {
        ret = recv(socket, buf, MESSAGE_BODY_SIZE, 0);
        if(ret <  0)
        {
            if( (errno == EAGAIN) || (errno == EWOULDBLOCK ) )
            {
                INFO_LOG("read later ");
                break;
            }
        }
        else if(ret == 0)
        {
            if(!SetEpollEvent(EPOLL_CTL_DEL, EPOLLERR, socket))
            {
                ERROR_LOG("SetEpollEvent error");
            }
            DataManager::GetInstance()->ClientInfoMapErase(socket);
            close(socket);
            break;
        }
        else
        {
            memcpy(recv_buf, buf, ret);
            len += ret;
        }
        memset(buf, 0, MESSAGE_BODY_SIZE);
    }

    if(len == MESSAGE_BODY_SIZE)
    {
        if(!SetEpollEvent(EPOLL_CTL_MOD, EPOLLOUT, socket))
        {
            ERROR_LOG("SetEpollEvent error");
        }
    }

    return len;
}

int Socket::WriteData(int socket, const char* send_buf)
{
    int len = 0;
    int ret = 0;
    while(len < MESSAGE_BODY_SIZE)
    {
        ret = send(socket, send_buf + len, MESSAGE_BODY_SIZE, 0);
        if(ret <= 0)
        {
            if(!SetEpollEvent(EPOLL_CTL_DEL, EPOLLERR, socket))
            {
                ERROR_LOG("SetEpollEvent error");
            }
            close(socket);
            break;
        }
        else
        {
            len += ret;
        }
    }

    if(len == MESSAGE_BODY_SIZE)
    {
        if(!SetEpollEvent(EPOLL_CTL_MOD, EPOLLIN, socket))
        {
            ERROR_LOG("SetEpollEvent error");
        }
    }

    return len;
}

int Socket::ServiceAccept()
{
    sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t clilen = sizeof(struct sockaddr);
    int new_sock = accept(m_service_sock, (struct sockaddr*)&client_addr, &clilen);

    if(!SetEpollEvent(EPOLL_CTL_ADD, EPOLLIN, new_sock))
    {
        ERROR_LOG("SetEpollEvent error");
    }
    std::string ip(inet_ntoa(client_addr.sin_addr));
    DataManager::GetInstance()->ClientInfoMapInsert(new_sock, "", client_addr.sin_port, ip);
    return new_sock;
}

bool Socket::SetEpollEvent(int op, int events, int socket)
{
    bool ret = false;
    struct epoll_event ev;
    ev.data.fd = socket;
    ev.events  = events;

    if(epoll_ctl(m_epfd, op, socket, &ev) == 0)
    {
        ret = true;
    }
    return ret;
}

void Socket::SendMsg(int socket)
{
    std::string sdk_id = DataManager::GetInstance()->ClientInfoMapFind(socket);
    char recv_msg[MESSAGE_BODY_SIZE] = {'\0'};

    while(DataManager::GetInstance()->CBQueueSize(socket) > 0)
    {   
        INFO_LOG("CBQueueSize  =  " << DataManager::GetInstance()->CBQueueSize(socket));
        memset(recv_msg, 0, MESSAGE_BODY_SIZE);
        DataManager::GetInstance()->CBQueueBack(socket, recv_msg);
        WriteData(socket, recv_msg);
    }
    while(DataManager::GetInstance()->CallBackQueueSize(sdk_id) > 0)
    {
        INFO_LOG("CallBackQueuesize  =  " << DataManager::GetInstance()->CallBackQueueSize(sdk_id)<<" sdk_id = " << sdk_id);
        memset(recv_msg, 0, MESSAGE_BODY_SIZE);
        DataManager::GetInstance()->CallBackQueueBack(sdk_id, recv_msg);
        WriteData(socket, recv_msg);
    }
}
}