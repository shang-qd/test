#pragma once

#ifndef _COMM_INFO_H
#define _COMM_INFO_H

#include <random>

class comm_info
{
public:
	// 数据id
	int id;
	// 车速
	//float m_vehicle_speed;
	// 发动机转速
	float m_engine_speed;
	// 节气阀开度
	float m_throttle_opening;
	// GPS方向角
	int m_gps_angle;
	// 超速报警
	bool m_speed_warning;
	// 疲劳驾驶
	bool m_fatigue;
	// 急转弯
	bool m_swerve;
	// 急加速
	bool m_add_speed;
	// 急减速
	bool m_sub_speed;
	// 碰撞
	bool m_impact;

	comm_info(int _id);

	// 随机生成
	void rand();

	// 得到危险值
	int get_danger() const;

	// 得到全部值
	int get_all() const;
};

#endif