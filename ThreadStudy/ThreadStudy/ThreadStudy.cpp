// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>  
#include <thread>  
#include <mutex>  
#include <condition_variable>  

using namespace std;

static mutex m;
static condition_variable cond;
static int LOOP = 10;
static int flag = 0;

void fun(int id)
{
	for (int i = 0; i < LOOP; i++)
	{
		unique_lock<mutex> lk(m);
		//一定要用循环判断，若是if多个阻塞线程唤醒后同时处于临界区
		while (id != flag)  
			cond.wait(lk);
		char c = 'A' + id;
		cout << c << endl;
		flag = (flag + 1) % 3;
		cond.notify_all();
	}
}


int _tmainabc(int argc, _TCHAR* argv[])
{
	thread B(fun, 1);
	thread C(fun, 2);
	fun(0);
	cout << endl;
	B.join();
	C.join();
	return 0;
}