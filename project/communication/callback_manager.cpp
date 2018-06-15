/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "callback_manager.h"
# include "data_manager.h"
#include "zmq/zmq.hpp"
#include "log.h"

namespace walletfront {

CallBackManager::CallBackManager()
{
    this->m_port = "5555";
    this->m_ip = "tcp://localhost:5555";

}

CallBackManager::~CallBackManager()
{

}

void CallBackManager::ZmqSub()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect(m_ip.c_str());
    const char *filter = "";

    subscriber.setsockopt( ZMQ_SUBSCRIBE, filter, strlen(filter));

    while(1)
    {
        zmq::message_t rev;
        subscriber.recv(&rev);

        if(rev.size() == MESSAGE_BODY_SIZE && rev.data() != NULL)
        {
            DataManager::GetInstance()->CallBackQueuePush((char *)rev.data());
            OnResponse((char *)rev.data());
        }
    }
}

void CallBackManager::OnResponse(const char *message)
{
    if(message == NULL)
    {
        FATAL_LOG("message == NULL");
        return;
    }

    std::string str(message);
    TRACE_LOG("message = " <<  str);
}

}