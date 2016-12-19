// C++.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <stdio.h>

class A
{
public:
	A()
	{
		a = 10;
	}
	void abc()
	{
		printf("%d \n", a);
	}
	int a;
};

extern "C" __declspec(dllexport)  A* _stdcall  Add()
{
	A *a = new  A();
	return a;
}

extern "C" __declspec(dllexport) void _stdcall cprintf(A *p)
{
	p->abc();
}

