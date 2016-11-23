// DangerLogic.cpp : 定义控制台应用程序的入口点。
//

#include "DangerLogic.h"
#include "comm_info.h"
#include "real_time_data.h"



JNIEXPORT jfloat JNICALL Java_DangerLogic_anger_1rate
(JNIEnv *, jobject, jlong id, jfloat engine_speed, jfloat throttle_opening, jint gps_angle, 
	jboolean speed_warning, jboolean fatigue, jboolean swerve, jboolean add_speed, jboolean sub_speed, jboolean impact)
{
	comm_info ci(id);
	ci.m_engine_speed = static_cast<decltype(ci.m_engine_speed)>(engine_speed);
	ci.m_throttle_opening = static_cast<decltype(ci.m_throttle_opening)>(throttle_opening);
	ci.m_gps_angle = static_cast<decltype(ci.m_gps_angle)>(gps_angle);
	ci.m_speed_warning = static_cast<decltype(ci.m_speed_warning)>(speed_warning);
	ci.m_fatigue = static_cast<decltype(ci.m_fatigue)>(fatigue);
	ci.m_swerve = static_cast<decltype(ci.m_swerve)>(swerve);
	ci.m_add_speed = static_cast<decltype(ci.m_add_speed)>(add_speed);
	ci.m_sub_speed = static_cast<decltype(ci.m_sub_speed)>(sub_speed);
	ci.m_impact = static_cast<decltype(ci.m_impact)>(impact);
	return anger_rate(id, ci.get_danger(), ci.get_all());
}

JNIEXPORT void JNICALL Java_DangerLogic_close_1evaluate
(JNIEnv *, jobject, jlong id)
{
	close_evaluate(id);
}
/*
int main(int argc, char* argv[])
{
	return 0;
}*/
