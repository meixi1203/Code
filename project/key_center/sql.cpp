/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#include "sql.h"
#include "log.h"
#include <vector>

namespace walletfront {

Sql::Sql() : m_server_ip(""), m_user_name(""), m_password(""), m_database_name("")
{
}

Sql::~Sql()
{
    FreeResult();
    if(m_sql_conn)
    {
        mysql_close(m_sql_conn);
    }
}

void Sql::SqlInit()
{
    m_server_ip     = "47.98.51.246";
    m_user_name     = "wallet_user";
    m_password      = "yx20180301@";
    m_database_name = "key_center";

    m_sql_conn = mysql_init(NULL);
    if(m_sql_conn == NULL)
    {
        ERROR_LOG("m_sql_conn == NULL");
        exit(1);
    }

    if(!mysql_real_connect(m_sql_conn, m_server_ip.c_str(), m_user_name.c_str(), m_password.c_str(), m_database_name.c_str(), 3306, NULL, 0))
    {
        ERROR_LOG("connect error  = "<< mysql_errno(m_sql_conn));
        ERROR_LOG("connect error  = "<< mysql_error(m_sql_conn));
        exit(1);
    }
    else
    {
        //INFO_LOG("Sql connect ok ");
    }
}

//quey
bool Sql::select(const std::string &operation)
{
    //INFO_LOG("mysql_query  sql  = " << operation);

    if(mysql_query(m_sql_conn, operation.c_str()))
    {
        ERROR_LOG("mysql_query error  = "<< mysql_errno(m_sql_conn));
        ERROR_LOG("mysql_query error  = "<< mysql_error(m_sql_conn));
        return false;
    }

    m_sql_res = mysql_use_result(m_sql_conn);

    return true;
}

//insert
bool Sql::insert(const std::string &operation)
{
    INFO_LOG("insert  sql = "<< operation );
    if(mysql_query(m_sql_conn, operation.c_str()))
    {
        ERROR_LOG("insert error  = "<< mysql_errno(m_sql_conn));
        ERROR_LOG("insert error  = "<< mysql_error(m_sql_conn));
        return false;
    }
    return true;
}

//update
bool Sql::update(const std::string &operation)
{
    INFO_LOG("update  sql = "<< operation );
    if(mysql_query(m_sql_conn,operation.c_str()))
    {
        ERROR_LOG("update error  = "<< mysql_errno(m_sql_conn));
        ERROR_LOG("update error  = "<< mysql_error(m_sql_conn));
        return false;
    }
    return true;
}

MYSQL_ROW Sql::GetRow()
{
    return mysql_fetch_row(m_sql_res);
}

//free MYSQL_RES
void Sql::FreeResult(void)
{
    mysql_free_result(m_sql_res);
}

//field num
int Sql::GetFieldNum(void)
{
    return mysql_field_count(m_sql_conn);
}

int Sql::GetColNum(void)
{
    return mysql_num_fields(m_sql_res);
}

void Sql::PrintResult(void)
{
    if(m_sql_res == NULL)
    {
        ERROR_LOG("Sql::PrintResult  m_sql_res == NULL");
        return;
    }

    int num = GetFieldNum();
    std::vector<std::string> field_name;

    for(int i = 0; i < num; i++)
    {
        field_name.push_back(mysql_fetch_field(m_sql_res)->name);
    }

    while(m_sql_row = GetRow())
    {
        for(int i = 0; i < num; i++)
        {
            INFO_LOG(field_name[i] << " = " << m_sql_row[i]);
        }
    }
}
}