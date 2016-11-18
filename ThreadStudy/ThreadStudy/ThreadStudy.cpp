// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <thread>  
#include <mutex>  
#include <condition_variable>  

using namespace std;

std::mutex g_mutex;
// 空位置的数量
std::condition_variable g_solt;
// 数据位置的数量
std::condition_variable g_item;

std::vector<int> vi;

bool f_solt()
{
	if (vi.size() < 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool f_item()
{
	if (vi.size() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void subf_insert(int item)
{
	std::unique_lock <std::mutex> lck(g_mutex);
	// 如果条件为真就不睡眠
	g_solt.wait(lck,f_solt);
	vi.push_back(item);
	g_item.notify_one();
}

int subf_remove()
{
	std::unique_lock <std::mutex> lck(g_mutex);
	g_item.wait(lck,f_item);
	int item = vi[vi.size() - 1];
	vi.pop_back();
	g_solt.notify_one();
	return item;
}

mutex mutex_print;

void f1()
{
	int c = 0;
	while (true)
	{
		c++;
		int i = rand();
		subf_insert(i);
		//mutex_print.lock();
		//std::cout << this_thread::get_id() << "生产数据:" << i << "库存:" << vi.size() << endl;
		//mutex_print.unlock();
		//this_thread::sleep_for(chrono::seconds(1));
		if (c > 100000)
		{
			//mutex_print.lock();
			//std::cout << "??????" << std::endl;
			printf("??????? \n");
			//mutex_print.unlock();
			break;
		}
	}
}

void f2()
{
	int c = 0;
	while (true)
	{
		c++;
		int i = subf_remove();
		//mutex_print.lock();
		//std::cout << this_thread::get_id() << "消费数据:" << i  << "库存:" << vi.size() << endl;
		//mutex_print.unlock();
		//this_thread::sleep_for(chrono::seconds(1));
		if (c > 200000)
		{
			//mutex_print.lock();
			printf("------------ \n");
			//std::cout << "-----" << std::endl;
			//mutex_print.unlock();
			break;
		}
	}
}

int _tmain__(int argc, _TCHAR* argv[])
{
	std::cout << "...." << endl;
	thread t1(f1);
	thread t3(f1);
	thread t2(f2);
	t1.join();
	t2.join();
	t3.join();
	std::cout << "++++++++++" << std::endl;
	return 0;
}