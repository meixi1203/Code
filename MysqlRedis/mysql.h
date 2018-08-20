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

	bool Open();  // ������
	void Close(); // �ر�����

	bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);       // ��ȡ����
	bool Query(const std::string& Querystr);     // ��������
	bool Update(const std::string& Querystr, const std::string& Updatastr);
	void ErrorIntoMySQL();       // ������Ϣ

private:
	MySQLConInfo MysqlConInfo;   // ������Ϣ
	MYSQL MysqlInstance;         // MySQL����
	MYSQL_RES *Result;           // ���ڴ�Ž�� 
};