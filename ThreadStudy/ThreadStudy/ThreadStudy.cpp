// ThreadStudy.cpp : �������̨Ӧ�ó������ڵ㡣
//
// C11 �����ܽ�

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm> 
#include <assert.h>
#include <list>

class String
{
public:
	String(const char *str = nullptr)
	{
		printf("String1 \n");
		if (str == nullptr)
		{
			m_data = new char[1]; 
			*m_data = '\0';
		}
		else
		{
			int length = strlen(str);
			m_data = new char[length + 1];  
			strcpy(m_data, str);
		}
	}

	// �������캯��
	String(const String &other)
	{
		printf("String2 \n");
		int length = strlen(other.m_data);
		m_data = new char[length + 1]; 
		strcpy(m_data, other.m_data);
	}

	~String(void)
	{
		printf("~String \n");
		delete[] m_data;
	}
	
	// �������캯��
	// ����& �ٵ���һ���������캯��
	String& operator = (const String &other)
	{
		printf(" = \n");
		if (this == &other)
			return *this;
		delete[] m_data;
		int length = strlen(other.m_data);
		m_data = new char[length + 1];
		strcpy(m_data, other.m_data);
		return *this;
	}

	void f()
	{
	}
private:
	char *m_data;// ���ڱ����ַ���  
};

// & 1���죬1���������죬1������
// 1���죬1���������죬1������
String GetA()
{
	String a;
	return a;
}

// & 1�����죬2��������1 ��ֵ����������
// 1�����죬1���������죬3��������1 ��ֵ����������
void f()
{
	// ����������
	String a;
	a.f();
	//String a2;
	a = GetA();
	a.f();
}

using namespace std;

class obj
{
public:
	obj() { cout << ">> create obj " << endl; }
	obj(const obj& other) { cout << ">> copy create obj " << endl; }
};

vector<obj> foo()
{
	vector<obj> c;
	c.push_back(obj());
	cout << "---- exit foo ----" << endl;
	return c;
}

class A
{
public:
	A()
	{
		printf("A() \n");
	}
	~A()
	{
		printf("~A() \n");
	}
	// ����������۸ߣ���Ҫ���ƶ���
	A(const A& other)
	{
		printf("A(const A& other) \n");
	}

	// ����������۵ͣ������ǽ�������
	A(A&& other)
	{
		printf("A(A&& other) \n");
	}
};

std::list<A> va;

int _tmain(int argc, _TCHAR* argv[])
{
	A a;
	// ��ֵ�Զ�����A(const A& other)
	va.push_back(a);
	// ͨ������ת�Ƶ���A(A&& other)��a��������Ժ�Ҳ�ͷϵ��˲�������ʹ���ˣ�����ķ����г���Ա����
	va.push_back(std::move(a));
	//������֪������ֵ������Զ�����A(A&& other)����ʱ����Ҳ���Զ���ʧ�ˣ�û������һ�д���ķ���
	//������������ʱ����Ҳ��Ҫ����
	va.push_back(A());
	printf("main �˳� \n");
	return 0;
}