// ���г���󣬳�����������������
// ˵����stl���Ƕ��̰߳�ȫ��
// ��������г��������������������߼������ܴ󣬳��ֹ��ϵĸ��ʴ�󽵵�
// �����ڱ�����Ա���Ե����Ӷ��ɾ���һ�����ȶ��ĳ���
// �����������Ĵ����ʱ����ķ���̫���ˣ�����д��������������
// �Һܿ췢���Լ���ˮƽ���ﲻ���� �������ͼ�¼�����ɣ��ȵ��Ժ�����������������ս��

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
	cout << n << "���" << endl;
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
	printf("������� \n");
	return 0;
}