/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "wallet_front_socket.h"
#include "data_manager.h"
#include <thread>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>


namespace walletfront {

SocketClient::SocketClient()
{
        this->m_ip   = "192.168.1.82";
        this->m_port = 5555;
}

void SocketClient::SocketClientInit()
{
    ClientConnect();
    std::thread send_ths(&SocketClient::SendRun, this);
    send_ths.detach();
    std::thread recv_ths(&SocketClient::RecvRun, this);
    recv_ths.detach();
}

void SocketClient::SendRun()
{
    while(1)
    {
        if(DataManager::GetInstance()->ReqQueueSize()  > 0)
        {
            std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " DataManager::GetInstance()->ReqQueueSize() = " << DataManager::GetInstance()->ReqQueueSize() << std::endl;
            char sender_msg[MESSAGE_BODY_SIZE] = "";
            DataManager::GetInstance()->ReqQueueBack(sender_msg);
            write(sender_msg);
        }
        else
        {
            sleep(0.5);
        }
    }
}

void SocketClient::RecvRun()
{
    while(1)
    {
        char buffer[MESSAGE_BODY_SIZE] = "";
        int size = read(buffer);
        if(size ==MESSAGE_BODY_SIZE && buffer != NULL )
        {
            DataManager::GetInstance()->CallBackQueuePush(buffer);
        }
        else
        {
            sleep(0.5);
        }
    }
}

void SocketClient::ClientConnect()
{
    m_client_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));

    sock_addr.sin_family      = PF_INET;
    sock_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
    sock_addr.sin_port        = htons(m_port);

    int result = 0;
    connect(m_client_sock, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr));
    if(result < 0)
    {
        std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " connect error" << std::endl;
    }
}

SocketClient::~SocketClient()
{
    close(m_client_sock);
}


int SocketClient::read(char szBuffer[])
{
    int size = recv(m_client_sock, szBuffer, MESSAGE_BODY_SIZE, 0);
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " SocketClient::read = " << size << std::endl;
    return size;
}

void  SocketClient::write(const char* str)
{
    int size = send(m_client_sock, str, MESSAGE_BODY_SIZE , 0);
    std::cout << "file path: " << __FILE__ << " function name：" << __FUNCTION__ << " line: " << __LINE__ << " SocketClient::write = " << size << std::endl;
}
}