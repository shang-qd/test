// 运行程序后，出现了难于理解的现象
// 说明了stl不是多线程安全的
// 如果程序中出现了这样的情况，别的逻辑代码会很大，出现故障的概率大大降低
// 以至于被程序员忽略掉，从而成就了一个不稳定的程序
// 当调试这样的代码的时候，真的发现太难了，本想写个博客来发表下
// 我很快发现自己的水平还达不到。 这个问题就记录下来吧，等到以后条件合适了在来挑战他

#include "stdafx.h"

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

static mutex g_m;
static vector<int> vi;

static void fun(int n)
{
	for (int i = 0; i < 1000000; i++)
	{
		//g_m.lock();
		vi.push_back(i);
		//g_m.unlock();
		if (vi.size() > 0)
		{
			//g_m.lock();
			vi.pop_back();
			//g_m.unlock();
		}
	}
	cout << n << "完成" << endl;
}

#define num_threads 4

int _tmain(int argc, _TCHAR* argv[])
{
	// 
	std::thread t[num_threads];
	for (int i = 0; i < num_threads; i++)
	{
		t[i] = std::thread(fun, i);
	}
	for (auto& th : t)
	{
		th.join();
	}
	printf("程序完成 \n");
	return 0;
}