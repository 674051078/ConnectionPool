#include<iostream>
#include<mysql.h>
#include "pch.h"
using namespace std;
//#include "Connection.h"
#include "CommonConnectionpoll.h"
int main() {

#if 0
	Connection conn;
	conn.connect("127.0.0.1", 3306, "root", "123123", "chat");
	/*
	Connection conn;
	char sql[1024] = {0};
	sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
		"zhang san",20,"male");
	conn.connect("127.0.0.1",3306, "root", "123123", "chat" ) ;
	conn.update(sql) ;
	*/
	clock_t begin = clock();
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();//连接池只获取一次，后面每次从同一个连接池里拿连接
	
	thread t1([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123123", "chat");
			conn.update(sql);
			/*
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->update(sql);*
		}
		}
	);
	thread t2([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123123", "chat");
			conn.update(sql);
			/*
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->update(sql);*/
		}
		}
	); thread t3([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123123", "chat");
			conn.update(sql);
			/*
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->update(sql);*/
		}
		}
	); thread t4([]() {
		//ConnectionPool* cp = ConnectionPool::getConnectionPool();
		for (int i = 0; i < 2500; i++)
		{
			Connection conn;
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
			conn.connect("127.0.0.1", 3306, "root", "123123", "chat");
			conn.update(sql);
			/*
			char sql[1024] = { 0 };
			sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
				"zhang san", 20, "male");
		shared_ptr<Connection> sp = cp->getConnection();
		sp->update(sql);*/
		}
		}
	);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
#endif

	clock_t begin = clock();
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();//连接池只获取一次，后面每次从同一个连接池里拿连接
	for (int i = 0; i < 10000; i++)
	{
		//未使用连接池
	   /**/
	   Connection conn;
	   char sql[1024] = {0};
	   sprintf(sql,"insert into user(name,age,sex) values('%s',%d,'%s') ",
		   "zhang san",20,"male");
	   conn.connect("127.0.0.1",3306, "root", "123123", "chat" ) ;
	   conn.update(sql);
	}
	clock_t end = clock();
	cout << (end - begin) << "ms" << endl;


#if 0
	for (int i = 0; i < 10000; i++)
	{
		//未使用连接池
	   /*
	   Connection conn;
	   char sql[1024] = {0};
	   sprintf(sql,"insert into user(name,age,sex) values('%s',%d,'%s') ",
		   "zhang san",20,"male");
	   conn.connect("127.0.0.1",3306, "root", "123123", "chat" ) ;
	   conn.update(sql);*/

	   //使用连接池
	   /*
		shared_ptr<Connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
			"zhang san", 20, "male");
		//生成一条用于向数据库中插入数据的 SQL 语句。它使用 sprintf 函数将用户提供的姓名、年龄和性别插入到 SQL 语句的相应位置。
		//这样做的好处是能够动态构建 SQL 语句，将变量的值嵌入到 SQL 中，以实现灵活的数据库操作。
				//sp->connect("127.0.0.1", 3306, "root", "123123", "chat");
		sp->update(sql);*/
	}
#endif
	return 0;
}
