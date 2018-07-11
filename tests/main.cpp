/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "wallet_front_socket.h"
#include "callback_manager.h"
#include "send_manager.h"
#include "zmq/zmq.hpp"
#include <unistd.h>
#include <thread>

static void ZmqPub()
{
    zmq::context_t context(1);
    zmq::socket_t pub(context, ZMQ_PUB);
    pub.bind("tcp://192.168.111.128:5556");
    char sender_msg[MESSAGE_BODY_SIZE] = "hello world! I'm Pub";
    while(1)
    {
        zmq::message_t reply_ (4096);
        memcpy ((void *) reply_.data (), sender_msg, 4096);
        pub.send (reply_);
        sleep(1);
    }
}

int main(int argc, char **argv)
{
    //std::thread pub_ths(ZmqPub);
    //pub_ths.detach();
    
    walletfront::SocketClient client ;
    walletfront::CallBackManager cb_manager;
    walletfront::SendManager send_mamager;

    send_mamager.SendManagerInit();
    client.SocketClientInit();
    cb_manager.CallBackManagerInit();
    send_mamager.run();

    while(1)
    {
        sleep(1);
    }
    
    return 0;
}