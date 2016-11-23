#pragma once

#ifndef _REAL_TIME_DATA_H
#define _REAL_TIME_DATA_H

#include <map>
#include <chrono>

using namespace std;
using namespace chrono;

class real_time_data
{
public:
	typedef system_clock::time_point rtd_time;
	typedef std::map<int, real_time_data> data_map;
	static data_map g_rtd;
private:
	// ��ʼʱ��
	rtd_time begin_time;
	// ���һ��ʱ��
	rtd_time last_time;
	// Σ�մ���
	int danger_number;
	// ȫ������
	int all_number;

public:
	real_time_data();
	~real_time_data();

	void update(int danger, int all);

	int seconds() const;

	int last2now() const;

	// �õ�Σ����
	template<class data_type>
	data_type anger_rate() const;
};

// �ĵ�ʶ����
float anger_rate(int id, int danger, int all);

// �ر�����
void close_evaluate(int id);

#endif