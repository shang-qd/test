// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//
// C11 特性总结

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

	// 拷贝构造函数
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
	
	// 拷贝构造函数
	// 加上& 少掉了一个拷贝构造函数
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
	char *m_data;// 用于保存字符串  
};

// & 1构造，1个拷贝构造，1个析构
// 1构造，1个拷贝构造，1个析构
String GetA()
{
	String a;
	return a;
}

// & 1个构造，2个析构，1 赋值操作符重载
// 1个构造，1个拷贝构造，3个析构，1 赋值操作符重载
void f()
{
	// 这里有析构
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
	// 这个函数代价高，需要复制对象
	A(const A& other)
	{
		printf("A(const A& other) \n");
	}

	// 这个函数代价低，仅仅是交换对象
	A(A&& other)
	{
		printf("A(A&& other) \n");
	}
};

std::list<A> va;

int _tmain(int argc, _TCHAR* argv[])
{
	A a;
	// 左值自动调用A(const A& other)
	va.push_back(a);
	// 通过语意转移调用A(A&& other)，a这个对象以后也就废掉了不能正常使用了，后面的风险有程序员负责
	va.push_back(std::move(a));
	//编译器知道是右值，因此自动调用A(A&& other)，临时对象也就自动消失了，没有上面一行代码的风险
	//交换回来的临时对象也需要析构
	va.push_back(A());
	printf("main 退出 \n");
	return 0;
}