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
	// 开始时间
	rtd_time begin_time;
	// 最后一次时间
	rtd_time last_time;
	// 危险次数
	int danger_number;
	// 全部次数
	int all_number;

public:
	real_time_data();
	~real_time_data();

	void update(int danger, int all);

	int seconds() const;

	int last2now() const;

	// 得到危险率
	template<class data_type>
	data_type anger_rate() const;
};

// 的到识别率
float anger_rate(int id, int danger, int all);

// 关闭评估
void close_evaluate(int id);

#endif