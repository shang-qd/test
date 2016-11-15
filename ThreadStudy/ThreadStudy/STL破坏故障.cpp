// 运行程序后，出现了难于理解的现象
// 说明了stl不是多线程安全的
// 如果程序中出现了这样的情况，别的逻辑代码会很大，出现故障的概率大大降低
// 以至于被程序员忽略掉，从而成就了一个不稳定的程序

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <thread>

using namespace std;

static vector<int> vi;

static void fun(int n)
{
	for (int i = 0; i < 1000000; i++)
	{
		vi.push_back(i);
		if (vi.size() > 0)
		{
			vi.pop_back();
		}
	}
	cout << n << "完成" << endl;
}

#define num_threads 4

static int test_main(int argc, _TCHAR* argv[])
{
	std::thread t[num_threads];
	for (int i = 0; i < num_threads; i++)
	{
		t[i] = std::thread(fun, i);
	}
	for (int i = 0; i < num_threads; i++)
	{
		t[i].join();
	}
	printf("程序完成 \n");
	return 0;
}