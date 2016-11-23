//#include "stdafx.h"
#include "comm_info.h"

comm_info::comm_info(int _id) : id(_id)
{
}

void comm_info::rand()
{
	std::random_device rd;
	// �������������
	std::mt19937 gen(rd());
	// ��̫�ֲ�
	std::normal_distribution<float> nd(1.1f, 0.1f);
	std::uniform_int_distribution<> uid(0, 100);

	m_engine_speed = nd(gen);
	m_throttle_opening = nd(gen);

	float f = 95;
	// ����
	m_speed_warning = uid(gen) > f ? true : false;
	// ƣ�ͼ�ʻ
	m_fatigue = uid(gen) > f ? true : false;
	// ��ת��
	m_swerve = uid(gen) > f ? true : false;
	// ������
	m_add_speed = uid(gen) > f ? true : false;
	// ����
	m_sub_speed = uid(gen) > f ? true : false;
	// ��ײ
	m_impact = uid(gen) > f ? true : false;
}


int comm_info::get_danger() const
{
	int res = 0;
	// ������ת��(r/min)
	if (m_engine_speed > 1.3 || m_engine_speed < 0.9)
	{
		res += 1;
	}
	// ����������
	if (m_throttle_opening > 1.3 || m_throttle_opening < 0.9)
	{
		res += 1;
	}
	// ���ٱ���
	res += m_speed_warning ? 1 : 0;
	// ƣ�ͼ�ʻ
	res += m_fatigue ? 1 : 0;
	// ��ת��
	res += m_swerve ? 1 : 0;
	// ������
	res += m_add_speed ? 1 : 0;
	// ������
	res += m_sub_speed ? 1 : 0;
	// ��ײ
	res += m_impact ? 1 : 0;
	return res;
}

int comm_info::get_all() const
{
	int res = 0;
	// ������ת��(r/min
	res += 1;
	// ����������
	res += 1;
	// ���ٱ���
	res += 1;
	// ƣ�ͼ�ʻ
	res += 1;
	// ��ת��
	res += 1;
	// ������
	res += 1;
	// ������
	res += 1;
	// ��ײ
	res += 1;
	return res;
}