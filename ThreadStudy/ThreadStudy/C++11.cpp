// ThreadStudy.cpp : �������̨Ӧ�ó������ڵ㡣
//
// C11 �����ܽ�

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <list>
#include <map>
#include <string>
#include <memory>
//#include <functional>
#include <algorithm> 
#include <assert.h>

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
	// unique_ptr ���ܸ�ֵ������һ������û�п������캯������Ҫ������ô������move��
	// �������һ��ָ��ָ��һ���������unique_ptr������Ƕ��ָ��ָ��һ���������shared_ptr�������ʹ�������ü�����
	// weak_ptr:shared_ptr�����֡� ��������ֶ�ע���Ǹ�����,��Զ�����й۲죬���Ҿ����ܵĽ��ͺ�ɭ��ЧӦ

	std::unique_ptr<int> px1 = std::make_unique<int>(10);
	//������仰��������������
	//std::unique_ptr<int> px2 = px1;

	auto sp = std::make_shared<int>(10);

	assert(sp.use_count() == 1);
	std::weak_ptr<int> wp(sp);
	assert(wp.use_count() == 1);
	if (!wp.expired())
	{
		std::shared_ptr<int> sp2 = wp.lock();
		*sp2 = 100;
		assert(wp.use_count() == 2);
	}
	assert(wp.use_count() == 1);
}

void ����7()
{
	// �����������������ָ����ʹ�������������������ѵ�ʱ�����������ֵ��Ȼ��˯��ʱ���ֵ��������ֵ�Ѿ������ı䣩
	// ����������ָ��������
	std::vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(3);
	std::for_each(std::begin(v), std::end(v), [](int n) {std::cout << n << std::endl; });
	auto is_odd = [](int n) {return n % 2 == 1; };
	auto pos = std::find_if(std::begin(v), std::end(v), is_odd);
	if (pos != std::end(v))
	{
		std::cout << *pos << std::endl;
	}
}

void ����8()
{
	// �ǳ�Աbegin()��end()
	// �������Ӱɣ����ûʲô��˵��
	int arr[] = { 1, 2, 3 };
	std::for_each(std::begin(arr), std::end(arr), [](int n) {std::cout << n << std::endl; });
	auto is_odd = [](int n) {return n % 2 == 1; };
	auto pos = std::find_if(std::begin(arr), std::end(arr), is_odd);
	if (pos != std::end(arr))
		std::cout << *pos << std::endl;
}

template <typename T, size_t Size>
class Vector
{
	static_assert(Size > 3, "Size is too small");
	T _points[Size];
};

template <typename T1, typename T2>
T1 add(T1 t1, T2 t2)
{
	static_assert(std::is_integral<T1>::value, "Type T1 must be integral");
	static_assert(std::is_integral<T2>::value, "Type T2 must be integral");
	return t1 + t2;
}

void ����9()
{
	// static_assert�� type traits
	// ��������Ҫ�����ڱ���ʱ��(Ҳ��Ҫ���ڸ���֮��)���������������Ӱ�
	// 1
	Vector<int, 11> a1;
	//Vector<double, 2> a2;

	std::cout << add(1, 3) << std::endl;
	//std::cout << add(1.1, 2) << std::endl;
}

void ����10()
{
	// Move���� ��ϸ���������ƪ���İɣ�����˵�ĺ������
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
	A a;
	// ��ֵ�Զ�����A(const A& other)
	va.push_back(a);
	// ͨ������ת�Ƶ���A(A&& other)��a��������Ժ�Ҳ�ͷϵ��˲�������ʹ���ˣ�����ķ����г���Ա����
	va.push_back(std::move(a));
	//������֪������ֵ������Զ�����A(A&& other)����ʱ����Ҳ���Զ���ʧ�ˣ�û������һ�д���ķ���
	//������������ʱ����Ҳ��Ҫ����
	va.push_back(A());
}

/*
// ����������Ҫ�Ŀ⣬�߳̿��������ʽ
int _tmain(int argc, _TCHAR* argv[])
{
	����10();
	return 0;
}*/