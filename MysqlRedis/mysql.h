#pragma once

#include "mysql/mysql.h"
#include <iostream>
#include <vector>


typedef struct
{
	std::string server;
	std::string user;
	std::string password;
	std::string database;
	int port;
}MySQLConInfo;

class MySQLInterface
{
public:
	MySQLInterface();
	virtual ~MySQLInterface();
	bool MySQLInterfaceInit();

	bool Open();  // 打开连接
	void Close(); // 关闭连接

	bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);       // 读取数据
	bool Query(const std::string& Querystr);     // 其他操作
	bool Update(const std::string& Querystr, const std::string& Updatastr);
	void ErrorIntoMySQL();       // 错误消息

private:
	MySQLConInfo MysqlConInfo;   // 连接信息
	MYSQL MysqlInstance;         // MySQL对象
	MYSQL_RES *Result;           // 用于存放结果 
};