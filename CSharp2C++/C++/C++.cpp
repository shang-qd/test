// C++.cpp : ���� DLL Ӧ�ó���ĵ���������
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