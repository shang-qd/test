//#include "stdafx.h"
#include "comm_info.h"

comm_info::comm_info(int _id) : id(_id)
{
}

void comm_info::rand()
{
	std::random_device rd;
	// 高质量的随机数
	std::mt19937 gen(rd());
	// 正太分布
	std::normal_distribution<float> nd(1.1f, 0.1f);
	std::uniform_int_distribution<> uid(0, 100);

	m_engine_speed = nd(gen);
	m_throttle_opening = nd(gen);

	float f = 95;
	// 超速
	m_speed_warning = uid(gen) > f ? true : false;
	// 疲劳驾驶
	m_fatigue = uid(gen) > f ? true : false;
	// 急转弯
	m_swerve = uid(gen) > f ? true : false;
	// 急加速
	m_add_speed = uid(gen) > f ? true : false;
	// 急减
	m_sub_speed = uid(gen) > f ? true : false;
	// 碰撞
	m_impact = uid(gen) > f ? true : false;
}


int comm_info::get_danger() const
{
	int res = 0;
	// 发动机转速(r/min)
	if (m_engine_speed > 1.3 || m_engine_speed < 0.9)
	{
		res += 1;
	}
	// 节气阀开度
	if (m_throttle_opening > 1.3 || m_throttle_opening < 0.9)
	{
		res += 1;
	}
	// 超速报警
	res += m_speed_warning ? 1 : 0;
	// 疲劳驾驶
	res += m_fatigue ? 1 : 0;
	// 急转弯
	res += m_swerve ? 1 : 0;
	// 急加速
	res += m_add_speed ? 1 : 0;
	// 急减速
	res += m_sub_speed ? 1 : 0;
	// 碰撞
	res += m_impact ? 1 : 0;
	return res;
}

int comm_info::get_all() const
{
	int res = 0;
	// 发动机转速(r/min
	res += 1;
	// 节气阀开度
	res += 1;
	// 超速报警
	res += 1;
	// 疲劳驾驶
	res += 1;
	// 急转弯
	res += 1;
	// 急加速
	res += 1;
	// 急减速
	res += 1;
	// 碰撞
	res += 1;
	return res;
}