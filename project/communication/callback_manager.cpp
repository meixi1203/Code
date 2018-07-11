/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "callback_manager.h"
#include "data_manager.h"
#include "common.h"
#include "zmq/zmq.hpp"
#include "log.h"
#include <thread>

namespace walletfront {

CallBackManager::CallBackManager()
{

}

CallBackManager::~CallBackManager()
{

}

void CallBackManager::CallBackManagerInit()
{
    std::thread callback_thread(&CallBackManager::ZmqSub, this);
    callback_thread.detach();
}

void CallBackManager::ZmqSub()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(walletfront::Common::GetInstance()->GetPubService().c_str());
    const char *filter = "";

    subscriber.setsockopt( ZMQ_SUBSCRIBE, filter, strlen(filter));

    while(1)
    {
        zmq::message_t rev;
        subscriber.recv(&rev);
        if(rev.size() == MESSAGE_BODY_SIZE && rev.data() != NULL)
        {
            OnResponse((char *)rev.data());
        }
    }
}

void CallBackManager::OnResponse(const char *message)
{
    if(message == NULL)
    {
        ERROR_LOG("message == NULL");
        return;
    }
    //TODO
    //DataManager::GetInstance()->CallBackQueuePush("sdk_id", message);

}

}