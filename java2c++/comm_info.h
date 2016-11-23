#pragma once

#ifndef _COMM_INFO_H
#define _COMM_INFO_H

#include <random>

class comm_info
{
public:
	// ����id
	int id;
	// ����
	//float m_vehicle_speed;
	// ������ת��
	float m_engine_speed;
	// ����������
	float m_throttle_opening;
	// GPS�����
	int m_gps_angle;
	// ���ٱ���
	bool m_speed_warning;
	// ƣ�ͼ�ʻ
	bool m_fatigue;
	// ��ת��
	bool m_swerve;
	// ������
	bool m_add_speed;
	// ������
	bool m_sub_speed;
	// ��ײ
	bool m_impact;

	comm_info(int _id);

	// �������
	void rand();

	// �õ�Σ��ֵ
	int get_danger() const;

	// �õ�ȫ��ֵ
	int get_all() const;
};

#endif