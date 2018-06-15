/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "request_manager.h"
#include "wallet_front_socket.h"
#include "callback_manager.h"
#include "log.h"
#include <unistd.h>
#include <thread>

int main(int argc, char **argv)
{
    walletfront::Socket wallet_front_socket;
    walletfront::RequestManager req_manager;
    walletfront::CallBackManager callback_manager;

    TRACE_LOG("wallet service start");
    
    std::thread recv_thread(&walletfront::RequestManager::Run, req_manager);
    std::thread callback_thread(&walletfront::CallBackManager::ZmqSub, callback_manager);

    recv_thread.detach();
    callback_thread.detach();
    wallet_front_socket.ServiceStart();

    while(1)
    {
        wallet_front_socket.ServiceEpollRun();
    }

    return 0;
}