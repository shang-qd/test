#include "real_time_data.h"
#include <mutex>

static std::mutex g_mutex;

real_time_data::data_map real_time_data::g_rtd;

real_time_data::real_time_data()
{
	begin_time = system_clock::now();
	danger_number = 0;
	all_number = 0;
	last_time = begin_time;
}

real_time_data::~real_time_data()
{
}

void real_time_data::update(int danger, int all)
{
	danger_number += danger;
	all_number += all;
	last_time = system_clock::now();
}

int real_time_data::seconds() const
{
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(last_time - begin_time);
	return static_cast<int>(duration.count());
}

int real_time_data::last2now() const
{
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(system_clock::now() - last_time);
	return static_cast<int>(duration.count());
}

// 得到危险率
template<class data_type>
data_type real_time_data::anger_rate() const
{
	return static_cast<data_type>(danger_number) / static_cast<data_type>(all_number);
}

float anger_rate(int id, int danger, int all)
{
	std::unique_lock<std::mutex> lck(g_mutex);
	auto item = real_time_data::g_rtd.find(id);
	if (item == real_time_data::g_rtd.end())
	{
		real_time_data obj;
		obj.update(danger, all);
		real_time_data::g_rtd.insert(pair<int, real_time_data>(id, move(obj)));
		return obj.anger_rate<float>();
	}
	else
	{
		item->second.update(danger, all);
		return item->second.anger_rate<float>();
	}
}

// 关闭评估
void close_evaluate(int id)
{
	std::unique_lock<std::mutex> lck(g_mutex);
	auto item = real_time_data::g_rtd.find(id);
	if (item != real_time_data::g_rtd.end())
	{
		real_time_data::g_rtd.erase(item);
	}
}