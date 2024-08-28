#include"pch.h"
#include"public.h"
#include"CommonConnectionpoll.h"
#include<iostream>
using namespace std;

//线程安全的懒汉式单例函数接口
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool poll;//静态函局部变量初始化  编译器自动lock和unlock
	return &poll;
}

//从配置文件中加载配置项
bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");//只读取mysql.ini文件
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist;");
		return false;
	}
	while (!feof(pf))
		//feof(pf) 函数检查是否到达文件末尾，但它并不总是可靠的，
		// 可能导致读取额外的行。建议使用 fgets 函数的返回值来控制循环。
	{
		char line[1024] = { 0 }; //定义一个字符数组 line 用于存储从文件中读取的一行文本。大小为 1024 字节。
		fgets(line, 1024, pf);//从文件 pf 中读取一行文本，并存储到 line 数组中。
		// fgets 会读取一行并包括换行符，最多读取 1024 字节（包括结尾的 \0）

		string str = line; //将 C 风格的字符串 line 转换为 C++ 的 std::string 对象 str，以便使用 std::string 的方法进行处理。
		int idx = str.find("=", 0);//从字符串 str 的第 0 个位置开始查找等号 = 的位置。
		if (idx == -1)
		{
			continue;
		}
		int endidx = str.find("\n", idx);//从字符串 str 的 idx 位置开始查找换行符 \n 的位置。
		string key = str.substr(0, idx);//取0-idx之间的字符
		string value = str.substr(idx + 1, endidx - idx - 1);//取(idx + 1)--(endidx - idx -1)之间的字符

		if (key == "ip")
		{
			_ip = value;
		}
		if (key == "port")
		{
			_port = atoi(value.c_str());
		}
		else if (key == "username")
		{
			_username = value;
		}
		else if (key == "password")
		{
			_password = value;
		}
		else if (key == "dbname")
		{
			_dbname = value;
		}
		else if (key == "initSize")
		{
			_initSize = atoi(value.c_str());
			//atoi 是 C 标准库中的一个函数，全名是 "ASCII to Integer"。
			// 它用于将 C 风格的字符串（const char*）转换为 int 类型的整数。
		}
		else if (key == "maxSize") {
			_maxSize = atoi(value.c_str());
		}
		else if (key == "maxIdleTime") {
			_maxIdleTime = atoi(value.c_str());
		}
		else if (key == "connectionTimeOut") {
			_connectionTimeout = atoi(value.c_str());
		}
	}
	
	fclose(pf);
	return true;
/*
示例：
假设 value 是一个字符串 "3306"，这段代码的执行过程如下：
1.value.c_str()：value 是 "3306"，value.c_str() 返回一个 const char* 指向 "3306" 的字符串。
2.atoi(value.c_str())：atoi 将 "3306" 转换为整数 3306。
3.赋值操作：_port = 3306，将 3306 存储在 _port 成员变量中。
*/
}

//连接池的构造
ConnectionPool::ConnectionPool()
{
	//加载配置项
	if (!loadConfigFile())
	{
		return;
	}
	//创建初始数量的 连接池的连接
	for (int i = 0; i < _initSize; i++)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();//刷新一下开始空闲的起始时间
		_connectionQue.push(p);//进入队列
		_connectionCnt++;
	}
	//启动一个新线程，作为连接的生产者 linux thread =>pthread_create
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));//依赖ConnectionPool ，使用this
	produce.detach();

	//启动一个新的定时线程，扫描超过maxIdeTime时间的空闲连接，进行对于连接到回收
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//运行在独立线程中，专门负责生产新连接
void ConnectionPool::produceConnectionTask()
{
	//***********重点*******
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			cv.wait(lock);//队列不空，生产线程进入等待状态
		}
		//连接数量未到达上限，继续创建新的连接
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();//刷新一下开始空闲的起始时间
			_connectionQue.push(p);//开始生产
			_connectionCnt++;
		}
		//通知消费者线程可以消费连接了
		cv.notify_all();
	}
}

//给外部提供接口，从连接池中获取一个可用的空闲连接
shared_ptr<Connection> ConnectionPool::getConnection()
{
	//***********重点***********
	//加锁控制队列，后面要操作队列
	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		//sleep 不适用sleep的原因是 他会指定休眠时间，在此时间段内不会进行消费
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)));
		{
			//队列空，生产线程进入等待状态，如果在此时间段内有生产，可以消费
			if (_connectionQue.empty())
			{
				LOG("获取空闲连接超时了。获取连接失败！");
				return nullptr;
			}
		}
	}
	/*shared ptr智能指针析构时，会把connection资源直接delete掉，
	相当于调用connection的析构函数，connection就被close掉了。
	这里需要自定义shared_ptr释放资源的方式，把connection直接归还到queue当中*/
	shared_ptr<Connection> sp(_connectionQue.front(), [&](Connection* pcon) {
		//这里是在服务器应用线程中调用的，所以一定要考虑队列的线程安全操作
		unique_lock<mutex> lock(_queueMutex);
		pcon->refreshAliveTime();//刷新一下开始空闲的起始时间
		_connectionQue.push(pcon);
		});
	_connectionQue.pop();
	cv.notify_all();//谁消费了队列中的最后一个connection，谁负责通知生产者生产连接
	//消费完了以后，通知生产者线程检查一下，若队列为空，赶紧通知生产者生产线程

	return sp;
}

void ConnectionPool::scannerConnectionTask() {

	for (;;) {
		//通过sleep模拟定时效果
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		//扫描整个队列，释放多余的连接
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();//头出尾进
			if (p->getAliveTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;//调用~Connection()释放连接
			}
			else
			{
				break;//队头的连接没有超时(超过_maxIdleTime)，其他连接都不会超时
			}
		}
	}
}