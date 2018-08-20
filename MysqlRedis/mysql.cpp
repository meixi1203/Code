#include "mysql.h"
#include "log.h"

MySQLInterface::MySQLInterface()
{
	mysql_library_init(0, NULL, NULL);
	mysql_init(&MysqlInstance);

	// �����ַ����������޷���������
	mysql_options(&MysqlInstance, MYSQL_SET_CHARSET_NAME, "gbk");
}

MySQLInterface::~MySQLInterface()
{
	Close();
}

bool MySQLInterface::MySQLInterfaceInit()
{
	MysqlConInfo.server = "47.98.51.246";
	MysqlConInfo.user = "wallet_user";
	MysqlConInfo.password = "yx20180301@";
	MysqlConInfo.database = "key_center";
	MysqlConInfo.port = 3306;

	//SetMySQLConInfo();
	if (!Open())
	{
		ERROR_LOG("Mysql Open error");
	}
	return true;
}

// ������
bool MySQLInterface::Open()
{
	if (mysql_real_connect(&MysqlInstance, MysqlConInfo.server.c_str(), MysqlConInfo.user.c_str(),
		MysqlConInfo.password.c_str(), MysqlConInfo.database.c_str(), MysqlConInfo.port, 0, 0) != NULL)
	{
		INFO_LOG("Mysql Open Ok ");
		return true;
	}
	else
	{
		ErrorIntoMySQL();
		return false;
	}
}

// �Ͽ�����
void MySQLInterface::Close()
{
	mysql_close(&MysqlInstance);
}

//��ȡ����
bool MySQLInterface::Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data)
{
	if (0 != mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		ErrorIntoMySQL();
		return false;
	}

	Result = mysql_store_result(&MysqlInstance);

	// ������
	int row = mysql_num_rows(Result);
	int field = mysql_num_fields(Result);

	MYSQL_ROW line = NULL;
	line = mysql_fetch_row(Result);

	int j = 0;
	std::string temp;
	std::vector<std::vector<std::string> >().swap(data);
	while (NULL != line)
	{
		std::vector<std::string> linedata;
		for (int i = 0; i < field; i++)
		{
			if (line[i])
			{
				temp = line[i];
				linedata.push_back(temp);
			}
			else
			{
				temp = "";
				linedata.push_back(temp);
			}
		}
		line = mysql_fetch_row(Result);
		data.push_back(linedata);
	}
	return true;
}

// ��������
bool MySQLInterface::Query(const std::string& Querystr)
{
	if (0 == mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		return true;
	}
	ErrorIntoMySQL();
	return false;
}
bool MySQLInterface::Update(const std::string& Querystr, const std::string& Updatastr)
{
	return true;
}
//������Ϣ
void MySQLInterface::ErrorIntoMySQL()
{
	int ErrorNum = mysql_errno(&MysqlInstance); 
	std::string ErrorInfo = mysql_error(&MysqlInstance);
	ERROR_LOG("ErrorIntoMySQL  ErrorNum = " << ErrorNum << " ErrorInfo = " << ErrorInfo.c_str());
}