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
	cout << n << "���" << endl;
}

#define num_threads 4

int _tmainaa(int argc, _TCHAR* argv[])
{ 
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