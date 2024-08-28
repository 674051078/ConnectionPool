#pragma once
#include <string>
#include<mysql.h>
#include<ctime>
using namespace std;
/*
* 实现mysql的增删改查等操作
*/

class Connection
{
public:
	// 初始化数据库连接
	Connection();
	// 释放数据库连接资源
	~Connection();
	// 连接数据库
	bool connect(string ip,
		unsigned short port,
		string user,
		string password,
		string dbname);

	// 更新操作 insert、delete、update
	bool update(string sql);

	// 查询操作 select
	MYSQL_RES* query(string sql);
	//MYSQL_RES*：函数返回一个指向 MYSQL_RES 的指针，这个指针指向的是查询结果集。如果查询失败，可能返回 nullptr。

	//刷新一下连接的起始的空闲时间
	void refreshAliveTime() {_alivetime = clock();}
	//返回存活的时间  当前时间 - 刚开始起始连接,进入空闲状态后的时间
	clock_t getAliveTime()const { return clock() - _alivetime; }
private:
	MYSQL* _conn;//表示和mysql Server的一条连接
	clock_t _alivetime;//记录进入空闲时间状态后的起始存活时间
};