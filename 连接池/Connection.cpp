#include"pch.h"
#include"public.h"
#include "Connection.h"
#include<mysql.h>
#include<string>
#include<iostream>
using namespace std;

Connection::Connection()
{
	//��ʼ�����ݿ�����
		_conn = mysql_init(nullptr);
	//mysql_init() �� MySQL C API �е�һ�����������ڳ�ʼ��һ���µ� MySQL ���Ӿ����
	//��� mysql_init �ɹ���_conn ��ָ����Ч�� MYSQL �ṹ�����ʧ�ܣ�_conn ���� nullptr����ʾ��ʼ��ʧ�ܡ�
		if (_conn == nullptr)
		{
			LOG("MySQL ��ʼ��ʧ��");
		}
}

Connection::~Connection()
{
	// �ͷ����ݿ�������Դ
	if (_conn != nullptr)
		mysql_close(_conn);
}

bool Connection::connect(string ip, unsigned short port, string user, string password,string dbname)
{
	if (_conn == nullptr)
	{
		LOG("MySQL ���Ӿ����Ч");
		return false;
	}
	// �������ݿ�
	MYSQL* p = mysql_real_connect(_conn, ip.c_str(), user.c_str(),
		password.c_str(), dbname.c_str(), port, nullptr, 0);
	////mysql_real_connect���������ӵ� MySQL ���ݿ������������һϵ�в����������ݿ��������
// IP���˿ڡ��û�������������ݿ����ơ�������һ�� MYSQL ָ���ʾ���ӳɹ������� nullptr ��ʾ����ʧ�ܡ�
	//.c_str()�����ڽ� C++ �� std::string ����ת��Ϊ C ���� const char* �ַ�����
// �Ա��� C API �������� mysql_real_connect�����ݡ�
	return p != nullptr;
}

bool Connection::update(string sql)
{

	//���²��� insert��delete��update
	if (mysql_query(_conn, sql.c_str()))
	{
		LOG("����ʧ��:" + sql);
		return false;
	}
	return true;
}

MYSQL_RES* Connection::query(string sql)
{
	// ��ѯ���� select
	if (mysql_query(_conn, sql.c_str()))
	//���� MySQL C API �е�һ������������ִ�� SQL ��䡣
	// ������ִ���������͵� SQL ��䣬������ѯ�����¡�����ȡ�
	{
		if (_conn == nullptr)
		{
			LOG("MySQL ���Ӿ����Ч");
			return nullptr;
		}
		LOG("��ѯʧ��:" + sql);
		return nullptr;
	}
	return mysql_use_result(_conn);
	//�����ݿ������������ѯ�����������һ�� MYSQL_RES* ָ�룬��ʾ��ѯ�������
	// ���û�н�������ѯʧ�ܣ����� nullptr��
}

/*
mysql_real_connect():�������ӵ� MySQL ���ݿ������������һϵ�в����������ݿ��������
IP���˿ڡ��û�������������ݿ����ơ�������һ�� MYSQL ָ���ʾ���ӳɹ������� nullptr ��ʾ����ʧ�ܡ�

mysql_query():MySQL C API �е�һ������������ִ�� SQL ��䡣
������ִ���������͵� SQL ��䣬������ѯ�����¡�����ȡ�


mysql_use_result():�� MySQL C API �е�һ�����������ڴ� MySQL �����������������
������һ��ָ�� MYSQL_RES �ṹ��ָ�룬�ýṹ��ʾ��ѯ�����

mysql_free_result(result):�ڴ������������ͷŽ����ռ�õ��ڴ棬�����ڴ�й©��
*/
