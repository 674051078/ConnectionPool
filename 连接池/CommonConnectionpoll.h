#pragma once
#include <string>
#include<mysql.h>
#include"Connection.h"
#include<mutex>
#include<queue>
#include<atomic>
#include<thread>
#include<memory>
#include<functional>
#include<condition_variable>
using namespace std;

/*
实现连接池功能模块*/
class ConnectionPool {
public:
	//获取连接池对象实例
	static ConnectionPool* getConnectionPool();
	//给外部提供接口，从连接池中获取一个可控的空闲连接
	shared_ptr<Connection> getConnection();//智能指针会在用户不使用的时候自动析构资源
	//Connection：这是智能指针所管理的对象类型。
private:
	//单例#1  构造函数私有化   用户无法在外部用new来构建对象
	ConnectionPool(); 

	//1.从配置文件中加载配置项
	bool loadConfigFile();

	//运行在独立的线程中，专门负责生产新连接
	void produceConnectionTask();

	//扫描超过maxIdeTime时间的空闲时间，进行对于连接到回收
	void scannerConnectionTask();

	string _ip; // mysql的ip地址
	unsigned short _port; // mysql的端口号3306
	string _username; // mysql登录用户名
	string _password; // mysql登录密码
	string _dbname;//连接的数据库名称
	int _initSize; // 连接池的初始连接量
	int _maxSize; // 连接池的最大连接量
	int _maxIdleTime; // 连接池最大空闲时间
	int _connectionTimeout; // 连接池获取连接的超时时间

	queue<Connection*>_connectionQue;//存储mysql连接的队列
	//Connection* 是一个指针类型，表示指向 Connection 类型对象的指针。
	//Connection 可能是一个类或结构体，封装了与连接（比如到MySQL数据库的连接）相关的细节。

	mutex _queueMutex;//维护连接队列的线程安全互斥锁
	//mutex是C++标准库的互斥锁，用于确保多个线程在同一时刻不会同时访问同一个共享资源，
	//互斥锁可以防止数据竞争，即多个线程在同一时间同时修改同一个数据从而导致数据不一致的问题。

	atomic_int _connectionCnt;//记录连接所创建的connecction连接的总数量
	//atomic_int 是C++标准库中的一个模板类，用于定义原子类型的整数。
	// 原子类型的操作是线程安全的，保证对该整数的操作（如加减）不会被中断或产生数据竞争。
	//_connectionCnt：这是一个用于记录 Connection 对象总数量的变量。因为它是原子类型的，
	// 多个线程可以同时安全地增加或减少这个计数，而不需要使用锁。

	condition_variable cv;//设置条件变量
	//condition_variable 是C++标准库中的一个条件变量，主要用于线程间的通信。
	//它可以让一个线程等待某个条件满足，然后再继续执行。同时，其他线程可以通知等待的线程，让它们继续执行。
	//cv：这是一个条件变量对象。它通常与 mutex 一起使用，用于让线程在等待某个条件时进入等待状态（阻塞），
	//直到另一个线程通知条件已满足。

};