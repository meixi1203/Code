/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <iostream>

namespace walletfront {

class RequestManager
{
    public:
        RequestManager();
        ~RequestManager();

    public:
        void Run();

    private:
        void OnRequest(const char *message);
        void HeartBeatHandler();
        
    private:
        void SendManage(char *message);

};
}

#endif