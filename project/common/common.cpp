/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "common.h"
#include "Config.h"
#include "log.h"

#define CONFIG_FILE_PATH "common/config.conf"

namespace walletfront {

Common::Common()
{
    InitServerFromConf();
}

Common::~Common()
{

}

void Common::InitServerFromConf()
{
    const char ConfigFile[] = CONFIG_FILE_PATH;
    Config configSettings(ConfigFile);
    std::string temp_str("");

    //get Wallet_server
    temp_str = configSettings.Read("Wallet_server", temp_str);
    int nPos = temp_str.find(':');
    m_conf_file.wallet_server_ip.assign(temp_str, 0, nPos);
    m_conf_file.wallet_server_port.assign(temp_str, nPos+1, temp_str.size());
    //get PULL_server
    temp_str = configSettings.Read("PUB_server", temp_str);
    m_conf_file.pull_server = temp_str;
    //get PUSH_server
    temp_str = configSettings.Read("PUSH_server", temp_str);
    m_conf_file.push_server = temp_str;
}

Common* Common::GetInstance()
{
    static Common *pCon = new Common();
    if(pCon == NULL)
    {
        INFO_LOG("GetInstance serror con == NULL ");
    }
    return pCon;
}

std::string Common::GetWalletServiceIP()
{
    if(!m_conf_file.wallet_server_ip.empty())
    {
        return m_conf_file.wallet_server_ip;
    }
    else
    {
        return "";
    }
}

std::string Common::GetWalletServicePort()
{
    if(!m_conf_file.wallet_server_port.empty())
    {
        return m_conf_file.wallet_server_port;
    }
    else
    {
        return "";
    }
}

std::string Common::GetPushService()
{
    if(!m_conf_file.push_server.empty())
    {
        return m_conf_file.push_server;
    }
    else
    {
        return "";
    }
}

std::string Common::GetPubService()
{
    if(!m_conf_file.pull_server.empty())
    {
        return m_conf_file.pull_server;
    }
    else
    {
        return "";
    }
}

}
