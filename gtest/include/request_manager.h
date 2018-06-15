/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef REQUEST_MANAGER_H
#define REQUEST_MANAGER_H

#include <iostream>

#define RESPONSE_BODY_SIZE 4096
namespace walletfront {

class RequestManager
{
    public:
        RequestManager();
        ~RequestManager();
        void OnRequest(const std::string rev_message);
        int add(int a, int b ){
            return a + b;
        }

    private:
        void SendManage(char *message);

};
}

#endif