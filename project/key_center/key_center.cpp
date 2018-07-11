/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "key_center.h"
#include "sql.h"
#include "log.h"

namespace walletfront {

KeyCenter::KeyCenter() : m_sql(NULL)
{
}

KeyCenter::~KeyCenter()
{
    if(m_sql)
    {
        delete m_sql;
        m_sql = NULL;
    }
}

void KeyCenter::KeyCenterInit()
{
    m_sql = new Sql();
    if(m_sql)
    {
        m_sql->SqlInit();
    }
    else
    {
        ERROR_LOG("m_sql == NULL");
    }
}

bool KeyCenter::CheckAppId(const std::string &app_id)
{
    if (m_sql == NULL || app_id.empty())
    {
        ERROR_LOG("m_sql == NULL");
        return false;
    }

    bool ret = false;
    MYSQL_ROW  sql_row;
    std::string sql = "select * from appid_record where appid = '"+app_id+"'";
    m_sql->select(sql);

    while(sql_row = m_sql->GetRow())
    {
        if(sql_row[1]  == app_id)
        {
            ret = true;
        }
    }
    return ret;
}

}
