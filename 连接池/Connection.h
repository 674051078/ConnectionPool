#pragma once
#include <string>
#include<mysql.h>
#include<ctime>
using namespace std;
/*
* ʵ��mysql����ɾ�Ĳ�Ȳ���
*/

class Connection
{
public:
	// ��ʼ�����ݿ�����
	Connection();
	// �ͷ����ݿ�������Դ
	~Connection();
	// �������ݿ�
	bool connect(string ip,
		unsigned short port,
		string user,
		string password,
		string dbname);

	// ���²��� insert��delete��update
	bool update(string sql);

	// ��ѯ���� select
	MYSQL_RES* query(string sql);
	//MYSQL_RES*����������һ��ָ�� MYSQL_RES ��ָ�룬���ָ��ָ����ǲ�ѯ������������ѯʧ�ܣ����ܷ��� nullptr��

	//ˢ��һ�����ӵ���ʼ�Ŀ���ʱ��
	void refreshAliveTime() {_alivetime = clock();}
	//���ش���ʱ��  ��ǰʱ�� - �տ�ʼ��ʼ����,�������״̬���ʱ��
	clock_t getAliveTime()const { return clock() - _alivetime; }
private:
	MYSQL* _conn;//��ʾ��mysql Server��һ������
	clock_t _alivetime;//��¼�������ʱ��״̬�����ʼ���ʱ��
};