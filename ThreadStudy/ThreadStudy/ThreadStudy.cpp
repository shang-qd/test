// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//
// C11 特性总结

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <map>
#include <string>

void 性质1()
{
	// 类型自动推导
	auto i = 42;        // i is an int
	auto l = 42LL;      // l is an long long

	// 下面的就很方面了
	std::vector<int> vi;
	for (std::vector<int>::iterator ite = vi.begin(); ite != vi.end(); ++ite)
	{
	}
	for (auto ite = vi.begin(); ite != vi.end(); ++ite)
	{
	}
}

void 性质2()
{
	// 空指针有宏提升为关键字，带来了更加安全的编译检查
	int res = 0;
	int *p1 = nullptr;
	int *p2 = NULL;
	// 不能编译通过
	//int i1 = nullptr;
	//可以编译通过
	//int i2 = NULL;
}

void 性质3()
{
	// 原来繁琐的循环现在变得简单多了
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


void 性质4()
{
	// 让不明确的重载变的明确
	// 这个经常变成考试程序员的一个头疼题目
	// 现在做了更加严格的检查，声明想重载，和不想重载
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

		//  输出的竟然是A
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
	// 上面的两个竟然输出了A
	{
		class A
		{
		public:
			virtual void f(short) { std::cout << "A::f" << std::endl; }
			// 不应当重载这个虚函数
			virtual void g(int) final { std::cout << "A::g" << std::endl; }
		};
		class B : public A
		{
		public:
			// 声明要重载，如果没有重载成功报编译错误
			virtual void f(short) override { std::cout << "B::f" << std::endl; }
			// 如果重载将会报错
			//virtual void g(int) { std::cout << "A::g" << std::endl; } // error C3248: “main::A::g”:  声明为“final”的函数无法被“main::B::g”重写
			// 确实没有重载
			virtual void g(float) { std::cout << "A::g" << std::endl; } // 重载
		};
	}
}

void 性质5()
{
	//强类型枚举 可以编译通过
	enum class N1 { None, One, All };
	enum class N2 { None, One, All };
	// 传统的方法出现重定义
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

void 性质6()
{
	// 智能指针
}

void 性质7()
{
	// 匿名函数
}

void 性质8()
{
	// 非成员begin()和end()
}

void 性质9()
{
	// static_assert和 type traits
}

void 性质10()
{
	// Move语义
}

int _tmain_(int argc, _TCHAR* argv[])
{
	性质6();
	return 0;
}