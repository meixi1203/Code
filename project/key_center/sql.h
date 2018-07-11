/*
# Copyright (c) 2018 Yixi. All rights reserved.
*/

#ifndef SQL_H
#define SQL_H

#include "mysql/mysql.h"
#include <iostream>

namespace walletfront {
class Sql
{
    public:
        Sql();
        ~Sql();

    public:
        void SqlInit();
        bool select(const std::string &operation);
        bool insert(const std::string &operation);
        bool update(const std::string &operation);
        int  GetColNum(void);
        int  GetFieldNum(void);
        void PrintResult(void);
        void FreeResult(void);
        MYSQL_ROW GetRow();

    private:
        std::string m_server_ip;
        std::string m_user_name;
        std::string m_password;
        std::string m_database_name;

    private:
        Sql(const Sql&);
        Sql& operator=(const Sql&);

    private:
        MYSQL*     m_sql_conn;
        MYSQL_RES* m_sql_res;
        MYSQL_ROW  m_sql_row;
};
}

#endif //SQL_H