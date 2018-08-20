#ifndef _REDIS_H_  
#define _REDIS_H_  
  
#include <iostream>  
#include <string>  
#include "include/hiredis.h"
  
class Redis  
{  
public:  
	static Redis* GetRedisInstance()
	{
		static Redis* redis = new Redis();
		if (redis)
		{
			return redis;
		}
		return NULL;
	}

    std::string get(std::string key)  
    {  
        this->_reply = (redisReply*)redisCommand(this->_connect, "GET %s", key.c_str());  
        std::string str = this->_reply->str;  
        freeReplyObject(this->_reply);  
        return str;  
    }  
  
    void set(std::string key, std::string value)  
    {  
        redisCommand(this->_connect, "SET %s %s", key.c_str(), value.c_str());  
    }

	void freeReply()
	{
		if (NULL != _reply)
		{
			::freeReplyObject(_reply);
			_reply = NULL;
		}
	}
private:
	Redis() : _host("127.0.0.1"), _port(6379)
	{
		if (connect(_host, _port))
		{
			std::cout << "Redis _connect OK!" << std::endl;
		}
	}

	~Redis()
	{
		if (NULL != _connect)
		{
			redisFree(_connect);
			_connect = NULL;
		}
		freeReply();
	}

	bool connect(std::string host, int port)
	{
		this->_connect = redisConnect(host.c_str(), port);
		if (this->_connect != NULL && this->_connect->err)
		{
			printf("connect error: %s\n", this->_connect->errstr);
			return false;
		}
		return true;
	}
  
private:  
    redisContext* _connect;  
    redisReply* _reply;
	std::string _host;
	int _port;
};  
#endif //_REDIS_H_  