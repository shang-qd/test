// C++.cpp : ���� DLL Ӧ�ó���ĵ���������
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

