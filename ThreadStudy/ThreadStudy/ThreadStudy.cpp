// ThreadStudy.cpp : 定义控制台应用程序的入口点。
//
// C11 特性总结

#include "stdafx.h"
#include <iostream>  
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm> 
#include <assert.h>
#include <list>


int _tmain(int argc, _TCHAR* argv[])
{
	// atomic 在硬件的支持下，为多线程提供了基本的原子操作
	// mutex 在atomic支持下为多线程提供了更长的原子尺度（为程序员抽象出来临界区）
	// condition_variable 在mutex的支持下为多线提供了更多的控制关系（为程序员抽象出来同步）
	return 0;
}