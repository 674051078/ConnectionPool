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
ʵ�����ӳع���ģ��*/
class ConnectionPool {
public:
	//��ȡ���ӳض���ʵ��
	static ConnectionPool* getConnectionPool();
	//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ���ɿصĿ�������
	shared_ptr<Connection> getConnection();//����ָ������û���ʹ�õ�ʱ���Զ�������Դ
	//Connection����������ָ��������Ķ������͡�
private:
	//����#1  ���캯��˽�л�   �û��޷����ⲿ��new����������
	ConnectionPool(); 

	//1.�������ļ��м���������
	bool loadConfigFile();

	//�����ڶ������߳��У�ר�Ÿ�������������
	void produceConnectionTask();

	//ɨ�賬��maxIdeTimeʱ��Ŀ���ʱ�䣬���ж������ӵ�����
	void scannerConnectionTask();

	string _ip; // mysql��ip��ַ
	unsigned short _port; // mysql�Ķ˿ں�3306
	string _username; // mysql��¼�û���
	string _password; // mysql��¼����
	string _dbname;//���ӵ����ݿ�����
	int _initSize; // ���ӳصĳ�ʼ������
	int _maxSize; // ���ӳص����������
	int _maxIdleTime; // ���ӳ�������ʱ��
	int _connectionTimeout; // ���ӳػ�ȡ���ӵĳ�ʱʱ��

	queue<Connection*>_connectionQue;//�洢mysql���ӵĶ���
	//Connection* ��һ��ָ�����ͣ���ʾָ�� Connection ���Ͷ����ָ�롣
	//Connection ������һ�����ṹ�壬��װ�������ӣ����絽MySQL���ݿ�����ӣ���ص�ϸ�ڡ�

	mutex _queueMutex;//ά�����Ӷ��е��̰߳�ȫ������
	//mutex��C++��׼��Ļ�����������ȷ������߳���ͬһʱ�̲���ͬʱ����ͬһ��������Դ��
	//���������Է�ֹ���ݾ�����������߳���ͬһʱ��ͬʱ�޸�ͬһ�����ݴӶ��������ݲ�һ�µ����⡣

	atomic_int _connectionCnt;//��¼������������connecction���ӵ�������
	//atomic_int ��C++��׼���е�һ��ģ���࣬���ڶ���ԭ�����͵�������
	// ԭ�����͵Ĳ������̰߳�ȫ�ģ���֤�Ը������Ĳ�������Ӽ������ᱻ�жϻ�������ݾ�����
	//_connectionCnt������һ�����ڼ�¼ Connection �����������ı�������Ϊ����ԭ�����͵ģ�
	// ����߳̿���ͬʱ��ȫ�����ӻ�������������������Ҫʹ������

	condition_variable cv;//������������
	//condition_variable ��C++��׼���е�һ��������������Ҫ�����̼߳��ͨ�š�
	//��������һ���̵߳ȴ�ĳ���������㣬Ȼ���ټ���ִ�С�ͬʱ�������߳̿���֪ͨ�ȴ����̣߳������Ǽ���ִ�С�
	//cv������һ����������������ͨ���� mutex һ��ʹ�ã��������߳��ڵȴ�ĳ������ʱ����ȴ�״̬����������
	//ֱ����һ���߳�֪ͨ���������㡣

};