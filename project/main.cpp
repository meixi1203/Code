/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "request_manager.h"
#include "wallet_front_socket.h"
#include "common.h"
#include "log.h"
#include <unistd.h>

static void SetCurrentDir()
{
    if(chdir("../../") < 0)
    {
        std::cout << "SetCurrentDir error " << std::endl;
        return;
    }
    char current_pwd[100] = {'\0'};
    getcwd(current_pwd, sizeof(current_pwd));

    printf("current dir :%s   \n" , current_pwd);
}

int main(int argc, char **argv)
{
    SetCurrentDir(); // It can only be set once

    INFO_LOG("wallet service start");
    INFO_LOG("wallet service ip " << walletfront::Common::GetInstance()->GetWalletServiceIP());
    INFO_LOG("wallet service port " << atoi(walletfront::Common::GetInstance()->GetWalletServicePort().c_str()));
    INFO_LOG("push service " << walletfront::Common::GetInstance()->GetPushService());
    INFO_LOG("pub service  " << walletfront::Common::GetInstance()->GetPubService());

    walletfront::Socket wallet_front_socket;
    walletfront::RequestManager req_manager;

    req_manager.RequestManagerInit();
    wallet_front_socket.SocketInit();

    while(1)
    {
        wallet_front_socket.EpollRun();
    }

    return 0;
}