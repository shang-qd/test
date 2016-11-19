// ThreadStudy.cpp : �������̨Ӧ�ó������ڵ㡣
//
// C11 �����ܽ�

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <map>
#include <string>

void ����1()
{
	// �����Զ��Ƶ�
	auto i = 42;        // i is an int
	auto l = 42LL;      // l is an long long

	// ����ľͺܷ�����
	std::vector<int> vi;
	for (std::vector<int>::iterator ite = vi.begin(); ite != vi.end(); ++ite)
	{
	}
	for (auto ite = vi.begin(); ite != vi.end(); ++ite)
	{
	}
}

void ����2()
{
	// ��ָ���к�����Ϊ�ؼ��֣������˸��Ӱ�ȫ�ı�����
	int res = 0;
	int *p1 = nullptr;
	int *p2 = NULL;
	// ���ܱ���ͨ��
	//int i1 = nullptr;
	//���Ա���ͨ��
	//int i2 = NULL;
}

void ����3()
{
	// ԭ��������ѭ�����ڱ�ü򵥶���
	std::map<std::string, std::vector<int>> map;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);

	map["one"] = v;

	for (const auto & kvp : map)
	{
		std::cout << kvp.first << std::endl;
		for (auto v : kvp.second)
		{
			std::cout << v << std::endl;
		}
	}
	int arr[] = { 1, 2, 3, 4, 5 };
	for (auto& e : arr)
	{
		e = e * e;
	}
}


void ����4()
{
	// �ò���ȷ�����ر����ȷ
	// ���������ɿ��Գ���Ա��һ��ͷ����Ŀ
	// �������˸����ϸ�ļ�飬���������أ��Ͳ�������
	{
		class A
		{
		public:
			virtual void f(short) { std::cout << "A::f" << std::endl; }
		};

		class B : public A
		{
		public:
			virtual void f(int) { std::cout << "B::f" << std::endl; }
		};

		//  ����ľ�Ȼ��A
		A  *p = new B();
		p->f(1);
	}
	{
		class A
		{
		public:
			virtual void f(int) const { std::cout << "A::f " << std::endl; }
		};
		class B : public A
		{
		public:
			virtual void f(int) { std::cout << "B::f" << std::endl; }
		};

		A  *p = new B();
		p->f(1);
	}
	// �����������Ȼ�����A
	{
		class A
		{
		public:
			virtual void f(short) { std::cout << "A::f" << std::endl; }
			// ��Ӧ����������麯��
			virtual void g(int) final { std::cout << "A::g" << std::endl; }
		};
		class B : public A
		{
		public:
			// ����Ҫ���أ����û�����سɹ����������
			virtual void f(short) override { std::cout << "B::f" << std::endl; }
			// ������ؽ��ᱨ��
			//virtual void g(int) { std::cout << "A::g" << std::endl; } // error C3248: ��main::A::g��:  ����Ϊ��final���ĺ����޷�����main::B::g����д
			// ȷʵû������
			virtual void g(float) { std::cout << "A::g" << std::endl; } // ����
		};
	}
}

void ����5()
{
	//ǿ����ö�� ���Ա���ͨ��
	enum class N1 { None, One, All };
	enum class N2 { None, One, All };
	// ��ͳ�ķ��������ض���
	/*
	enum  A1
	{
		None, One, All
	};
	enum  A2
	{
		None, One, All
	};*/
}

void ����6()
{
	// ����ָ��
}

void ����7()
{
	// ��������
}

void ����8()
{
	// �ǳ�Աbegin()��end()
}

void ����9()
{
	// static_assert�� type traits
}

void ����10()
{
	// Move����
}

int _tmain_(int argc, _TCHAR* argv[])
{
	����6();
	return 0;
}