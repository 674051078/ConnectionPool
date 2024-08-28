#include"pch.h"
#include"public.h"
#include "Connection.h"
#include<mysql.h>
#include<string>
#include<iostream>
using namespace std;

Connection::Connection()
{
	//初始化数据库连接
		_conn = mysql_init(nullptr);
	//mysql_init() 是 MySQL C API 中的一个函数，用于初始化一个新的 MySQL 连接句柄。
	//如果 mysql_init 成功，_conn 将指向有效的 MYSQL 结构；如果失败，_conn 将是 nullptr，表示初始化失败。
		if (_conn == nullptr)
		{
			LOG("MySQL 初始化失败");
		}
}

Connection::~Connection()
{
	// 释放数据库连接资源
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port, string user, string password,string dbname)
{
	if (_conn == nullptr)
	{
		LOG("MySQL 连接句柄无效");
		return false;
	}
	// 连接数据库
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	////mysql_real_connect：用于连接到 MySQL 数据库服务器，接收一系列参数包括数据库服务器的
// IP、端口、用户名、密码和数据库名称。它返回一个 MYSQL 指针表示连接成功，返回 nullptr 表示连接失败。
	//.c_str()：用于将 C++ 的 std::string 类型转换为 C 风格的 const char* 字符串，
// 以便与 C API 函数（如 mysql_real_connect）兼容。
	return p != nullptr;
}

bool Connection::update(string sql)
{

	//更新操作 insert、delete、update
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("更新失败:" + sql);
		return false;
	}
	return true;
}

MYSQL_RES* Connection::query(string sql)
{
	// 查询操作 select
	if (mysql_query(_conn, sql.c_str()))
	//这是 MySQL C API 中的一个函数，用于执行 SQL 语句。
	// 它可以执行任意类型的 SQL 语句，包括查询、更新、插入等。
	{
		if (_conn == nullptr)
		{
			LOG("MySQL 连接句柄无效");
			return nullptr;
		}
		LOG("查询失败:" + sql);
		return nullptr;
	}
	return mysql_use_result(_conn);
	//从数据库服务器检索查询结果，并返回一个 MYSQL_RES* 指针，表示查询结果集。
	// 如果没有结果集或查询失败，返回 nullptr。
}

/*
mysql_real_connect():用于连接到 MySQL 数据库服务器，接收一系列参数包括数据库服务器的
IP、端口、用户名、密码和数据库名称。它返回一个 MYSQL 指针表示连接成功，返回 nullptr 表示连接失败。

mysql_query():MySQL C API 中的一个函数，用于执行 SQL 语句。
它可以执行任意类型的 SQL 语句，包括查询、更新、插入等。


mysql_use_result():是 MySQL C API 中的一个函数，用于从 MySQL 服务器检索结果集。
它返回一个指向 MYSQL_RES 结构的指针，该结构表示查询结果。

mysql_free_result(result):在处理完结果集后，释放结果集占用的内存，避免内存泄漏。
*/
