/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "wallet_front_socket.h"
#include "data_manager.h"
#include "log.h"
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

namespace walletfront {

Socket::Socket()
{
    this->m_ip = "127.0.0.1";
    this->m_port = 6666;

    //reset m_events
    memset(&m_events, 0, sizeof(struct epoll_event)*20);  
}

Socket::~Socket()
{
    m_port = 8080;
    m_ip = "";
    close(m_epfd);
    close(m_service_sock);
}

void Socket::ServiceStart()
{
    ServiceBind();
    ServiceEpollPrepare();
}

void Socket::ServiceBind()
{
    if ((m_service_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        FATAL_LOG("ServiceBind  socket error");
    }

    sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family = PF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(this->m_ip.c_str());
    sock_addr.sin_port = htons(this->m_port);

    if (bind(m_service_sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr)) < 0)
    {
        FATAL_LOG("ServiceBind  bind error");
    }

    SetNBlock(m_service_sock);
    ServiceListen();
}

void Socket::ServiceListen()
{
    if (( listen(m_service_sock, 20) < 0))
    {
        FATAL_LOG("listen error");
    }
}

void Socket::SetNBlock(int sock)
{
    int opts = 0;

    opts = fcntl(sock, F_GETFL); //获取文件的flags值。
    if (opts<0)
    {
        FATAL_LOG("fcntl l(sock,GETFL) error");
    }

    opts = opts | O_NONBLOCK; //设置成非阻塞模式
    if (fcntl(sock ,F_SETFL, opts) <0 )
    {
        FATAL_LOG("fcntl(sock,SETFL,opts) error");
    }
}

void Socket::ServiceEpollPrepare()
{
    if ((m_epfd = epoll_create(20)) == -1)
    {
        FATAL_LOG("epoll_create error");
    }

    if(!SetEpollEvent(EPOLL_CTL_ADD, EPOLLIN, m_service_sock))
    {
        FATAL_LOG("SetEpollEvent error");
    }
}

void Socket::ServiceEpollRun()
{
    int nfds = epoll_wait(m_epfd, m_events, 20, -1);
    TRACE_LOG("epoll_wait  nfds = " << nfds );

    if (nfds == -1)
    {
        FATAL_LOG("epoll_wait error");
    }

    for (int i=0; i<nfds; ++i)
    {
        if (m_events[i].data.fd == m_service_sock)//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
        {
            ServiceAccept();
        }

        else if (m_events[i].events & EPOLLIN)//如果是已经连接的用户，并且收到数据，那么进行读入。
        {
            char recv_msg[MESSAGE_BODY_SIZE] = {'\0'};
            int len = ReadData(m_events[i].data.fd, recv_msg);
            TRACE_LOG("read end  len =  " << len );

            if(len == MESSAGE_BODY_SIZE && recv_msg != NULL)
            {
                DataManager::GetInstance()->RecvQueuePush(recv_msg);
            }
        }

        else if (m_events[i].events & EPOLLOUT) // 如果有数据发送
        {
            char reply_msg[MESSAGE_BODY_SIZE] = "reply hello world ";
            int len = WriteData(m_events[i].data.fd, reply_msg);
            TRACE_LOG("write end  len =  " << len );
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
        if(ret <= 0)
        {
            if(!SetEpollEvent(EPOLL_CTL_DEL, EPOLLERR, socket))
            {
                FATAL_LOG("SetEpollEvent error");
            }
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
                FATAL_LOG("SetEpollEvent error");
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
                FATAL_LOG("SetEpollEvent error");
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
            FATAL_LOG("SetEpollEvent error");
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
        FATAL_LOG("SetEpollEvent error");
    }

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

}