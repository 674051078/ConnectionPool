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
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();//���ӳ�ֻ��ȡһ�Σ�����ÿ�δ�ͬһ�����ӳ���������
	
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
	//ConnectionPool* cp = ConnectionPool::getConnectionPool();//���ӳ�ֻ��ȡһ�Σ�����ÿ�δ�ͬһ�����ӳ���������
	for (int i = 0; i < 10000; i++)
	{
		//δʹ�����ӳ�
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
		//δʹ�����ӳ�
	   /*
	   Connection conn;
	   char sql[1024] = {0};
	   sprintf(sql,"insert into user(name,age,sex) values('%s',%d,'%s') ",
		   "zhang san",20,"male");
	   conn.connect("127.0.0.1",3306, "root", "123123", "chat" ) ;
	   conn.update(sql);*/

	   //ʹ�����ӳ�
	   /*
		shared_ptr<Connection> sp = cp->getConnection();
		char sql[1024] = { 0 };
		sprintf(sql, "insert into user(name,age,sex) values('%s',%d,'%s') ",
			"zhang san", 20, "male");
		//����һ�����������ݿ��в������ݵ� SQL ��䡣��ʹ�� sprintf �������û��ṩ��������������Ա���뵽 SQL ������Ӧλ�á�
		//�������ĺô����ܹ���̬���� SQL ��䣬��������ֵǶ�뵽 SQL �У���ʵ���������ݿ������
				//sp->connect("127.0.0.1", 3306, "root", "123123", "chat");
		sp->update(sql);*/
	}
#endif
	return 0;
}
