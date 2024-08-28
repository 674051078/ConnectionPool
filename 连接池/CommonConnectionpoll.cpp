#include"pch.h"
#include"public.h"
#include"CommonConnectionpoll.h"
#include<iostream>
using namespace std;

//�̰߳�ȫ������ʽ���������ӿ�
ConnectionPool* ConnectionPool::getConnectionPool()
{
	static ConnectionPool poll;//��̬���ֲ�������ʼ��  �������Զ�lock��unlock
	return &poll;
}

//�������ļ��м���������
bool ConnectionPool::loadConfigFile()
{
	FILE* pf = fopen("mysql.ini", "r");//ֻ��ȡmysql.ini�ļ�
	if (pf == nullptr)
	{
		LOG("mysql.ini file is not exist;");
		return false;
	}
	while (!feof(pf))
		//feof(pf) ��������Ƿ񵽴��ļ�ĩβ�������������ǿɿ��ģ�
		// ���ܵ��¶�ȡ������С�����ʹ�� fgets �����ķ���ֵ������ѭ����
	{
		char line[1024] = { 0 }; //����һ���ַ����� line ���ڴ洢���ļ��ж�ȡ��һ���ı�����СΪ 1024 �ֽڡ�
		fgets(line, 1024, pf);//���ļ� pf �ж�ȡһ���ı������洢�� line �����С�
		// fgets ���ȡһ�в��������з�������ȡ 1024 �ֽڣ�������β�� \0��

		string str = line; //�� C �����ַ��� line ת��Ϊ C++ �� std::string ���� str���Ա�ʹ�� std::string �ķ������д���
		int idx = str.find("=", 0);//���ַ��� str �ĵ� 0 ��λ�ÿ�ʼ���ҵȺ� = ��λ�á�
		if (idx == -1)
		{
			continue;
		}
		int endidx = str.find("\n", idx);//���ַ��� str �� idx λ�ÿ�ʼ���һ��з� \n ��λ�á�
		string key = str.substr(0, idx);//ȡ0-idx֮����ַ�
		string value = str.substr(idx + 1, endidx - idx - 1);//ȡ(idx + 1)--(endidx - idx -1)֮����ַ�

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
			//atoi �� C ��׼���е�һ��������ȫ���� "ASCII to Integer"��
			// �����ڽ� C �����ַ�����const char*��ת��Ϊ int ���͵�������
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
ʾ����
���� value ��һ���ַ��� "3306"����δ����ִ�й������£�
1.value.c_str()��value �� "3306"��value.c_str() ����һ�� const char* ָ�� "3306" ���ַ�����
2.atoi(value.c_str())��atoi �� "3306" ת��Ϊ���� 3306��
3.��ֵ������_port = 3306���� 3306 �洢�� _port ��Ա�����С�
*/
}

//���ӳصĹ���
ConnectionPool::ConnectionPool()
{
	//����������
	if (!loadConfigFile())
	{
		return;
	}
	//������ʼ������ ���ӳص�����
	for (int i = 0; i < _initSize; i++)
	{
		Connection* p = new Connection();
		p->connect(_ip, _port, _username, _password, _dbname);
		p->refreshAliveTime();//ˢ��һ�¿�ʼ���е���ʼʱ��
		_connectionQue.push(p);//�������
		_connectionCnt++;
	}
	//����һ�����̣߳���Ϊ���ӵ������� linux thread =>pthread_create
	thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));//����ConnectionPool ��ʹ��this
	produce.detach();

	//����һ���µĶ�ʱ�̣߳�ɨ�賬��maxIdeTimeʱ��Ŀ������ӣ����ж������ӵ�����
	thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
	scanner.detach();
}

//�����ڶ����߳��У�ר�Ÿ�������������
void ConnectionPool::produceConnectionTask()
{
	//***********�ص�*******
	for (;;)
	{
		unique_lock<mutex> lock(_queueMutex);
		while (!_connectionQue.empty())
		{
			cv.wait(lock);//���в��գ������߳̽���ȴ�״̬
		}
		//��������δ�������ޣ����������µ�����
		if (_connectionCnt < _maxSize)
		{
			Connection* p = new Connection();
			p->connect(_ip, _port, _username, _password, _dbname);
			p->refreshAliveTime();//ˢ��һ�¿�ʼ���е���ʼʱ��
			_connectionQue.push(p);//��ʼ����
			_connectionCnt++;
		}
		//֪ͨ�������߳̿�������������
		cv.notify_all();
	}
}

//���ⲿ�ṩ�ӿڣ������ӳ��л�ȡһ�����õĿ�������
shared_ptr<Connection> ConnectionPool::getConnection()
{
	//***********�ص�***********
	//�������ƶ��У�����Ҫ��������
	unique_lock<mutex> lock(_queueMutex);
	while (_connectionQue.empty())
	{
		//sleep ������sleep��ԭ���� ����ָ������ʱ�䣬�ڴ�ʱ����ڲ����������
		if (cv_status::timeout == cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)));
		{
			//���пգ������߳̽���ȴ�״̬������ڴ�ʱ���������������������
			if (_connectionQue.empty())
			{
				LOG("��ȡ�������ӳ�ʱ�ˡ���ȡ����ʧ�ܣ�");
				return nullptr;
			}
		}
	}
	/*shared ptr����ָ������ʱ�����connection��Դֱ��delete����
	�൱�ڵ���connection������������connection�ͱ�close���ˡ�
	������Ҫ�Զ���shared_ptr�ͷ���Դ�ķ�ʽ����connectionֱ�ӹ黹��queue����*/
	shared_ptr<Connection> sp(_connectionQue.front(), [&](Connection* pcon) {
		//�������ڷ�����Ӧ���߳��е��õģ�����һ��Ҫ���Ƕ��е��̰߳�ȫ����
		unique_lock<mutex> lock(_queueMutex);
		pcon->refreshAliveTime();//ˢ��һ�¿�ʼ���е���ʼʱ��
		_connectionQue.push(pcon);
		});
	_connectionQue.pop();
	cv.notify_all();//˭�����˶����е����һ��connection��˭����֪ͨ��������������
	//���������Ժ�֪ͨ�������̼߳��һ�£�������Ϊ�գ��Ͻ�֪ͨ�����������߳�

	return sp;
}

void ConnectionPool::scannerConnectionTask() {

	for (;;) {
		//ͨ��sleepģ�ⶨʱЧ��
		this_thread::sleep_for(chrono::seconds(_maxIdleTime));
		//ɨ���������У��ͷŶ��������
		unique_lock<mutex> lock(_queueMutex);
		while (_connectionCnt > _initSize)
		{
			Connection* p = _connectionQue.front();//ͷ��β��
			if (p->getAliveTime() >= (_maxIdleTime * 1000))
			{
				_connectionQue.pop();
				_connectionCnt--;
				delete p;//����~Connection()�ͷ�����
			}
			else
			{
				break;//��ͷ������û�г�ʱ(����_maxIdleTime)���������Ӷ����ᳬʱ
			}
		}
	}
}