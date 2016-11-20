// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//
// C11 特性总结

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
	// unique_ptr 不能赋值给另外一个对象（没有拷贝构造函数），要真想这么做就用move吧
	// 如果就有一个指针指向一个对象就用unique_ptr，如果是多个指针指向一个对象就用shared_ptr（这个类使用了引用计数）
	// weak_ptr:shared_ptr的助手。 看这个名字都注定是个助手,想对对象进行观察，并且尽可能的降低海森堡效应

	std::unique_ptr<int> px1 = std::make_unique<int>(10);
	//下面这句话将会引起编译错误
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

void 性质7()
{
	// 如果是条件变量函数指针中使用了匿名函数，当唤醒的时候参数变量的值依然是睡眠时候的值（即便是值已经发生改变）
	// 这样可以用指针来代替
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

void 性质8()
{
	// 非成员begin()和end()
	// 来个例子吧，这个没什么好说的
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

void 性质9()
{
	// static_assert和 type traits
	// 这两个主要是用于编译时的(也主要用于高手之人)，就贴上两个例子吧
	// 1
	Vector<int, 11> a1;
	//Vector<double, 2> a2;

	std::cout << add(1, 3) << std::endl;
	//std::cout << add(1.1, 2) << std::endl;
}

void 性质10()
{
	// Move语义 仔细看下面的这篇博文吧，里面说的很清楚了
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
	A a;
	// 左值自动调用A(const A& other)
	va.push_back(a);
	// 通过语意转移调用A(A&& other)，a这个对象以后也就废掉了不能正常使用了，后面的风险有程序员负责
	va.push_back(std::move(a));
	//编译器知道是右值，因此自动调用A(A&& other)，临时对象也就自动消失了，没有上面一行代码的风险
	//交换回来的临时对象也需要析构
	va.push_back(A());
}

/*
// 还有两个重要的库，线程库和正则表达式
int _tmain(int argc, _TCHAR* argv[])
{
	性质10();
	return 0;
}*/