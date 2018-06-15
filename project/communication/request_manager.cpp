/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "request_manager.h"
#include "wallet_front_base.h"
#include "data_manager.h"
#include "front_engine.pb.h"
#include "zmq/zmq.hpp"
#include "log.h"
#include <thread>
#include <unistd.h>

namespace walletfront {

RequestManager::RequestManager()
{
    std::thread heart_thread(&RequestManager::HeartBeatHandler, this);
    heart_thread.detach();
}

RequestManager::~RequestManager()
{

}

void RequestManager::Run()
{
    while(1)
    {
        char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };
        if(!DataManager::GetInstance()->RecvQueueEmpty())
        {
            TRACE_LOG("Recv buff size  = " <<DataManager::GetInstance()->RecvQueueSize());
            DataManager::GetInstance()->RecvQueueBack(sender_msg);
            OnRequest(sender_msg);
        }
        else{
            sleep(1);
        }
    }
}

void RequestManager::OnRequest(const char *message)
{
    std::string sts(message);
    TRACE_LOG("OnRequest = " <<sts);
    
    
    #if 0
    FrontEngine::RequestMessage req_message;
    req_message.ParseFromArray(message, MESSAGE_BODY_SIZE);
    char sender_msg[MESSAGE_BODY_SIZE] = { '\0' };

    std::cout  << "message type = " << req_message.type() << std::endl;

    FrontEngine::RequestMessage reply_message;
    reply_message.SerializeToArray(sender_msg, sizeof(sender_msg));

    SendManage(sender_msg);
    #endif
}

void RequestManager::HeartBeatHandler()
{
    while(1)
    {
        int client_map_size = DataManager::GetInstance()->ClientInfoMapSize();
        for(int index = 0; index < client_map_size; index++)
         {
            int count = DataManager::GetInstance()->GetClientInfoMapCount(index);
            if(count == 5)
            {
                TRACE_LOG("client sdk close"<< DataManager::GetInstance()->GetClientInfoMapSdkId(index));
            }
            else if(count < 5 &&count > 0)
            {
                DataManager::GetInstance()->SetClientInfoMapCount(index);
            }
         }
        sleep(3);
        TRACE_LOG("sleep(3) client_map_size = "<< client_map_size);
    }

}

void RequestManager::SendManage(char *message)
{
    zmq::context_t context(1);
    zmq::socket_t sender(context, ZMQ_PUSH);
    sender.bind("tcp://*:5556"); // TBD

    int message_size = MESSAGE_BODY_SIZE;
    zmq::message_t *query = new zmq::message_t(message_size);
    memcpy(query->data(), message, message_size);

    sender.send(*query);
    TRACE_LOG("wallet front send ok");
}

}
