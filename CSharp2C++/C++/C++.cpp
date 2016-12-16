// C++.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#include <stdio.h>

extern "C" __declspec(dllexport)  int _stdcall  Add(int a, int b)
{
	return a + b;
}

extern "C" __declspec(dllexport) void _stdcall cprintf(char *p)
{
	printf("%s \n", p);
}