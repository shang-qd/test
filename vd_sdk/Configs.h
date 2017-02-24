#pragma once

#include <string>
#include <curl/curl.h>

#include "VcyberVoiceSDK.h"

class Configs {
public:
	// ��ѡ ��������ַ
	std::string m_server_addr;
	// ��ѡ Ӧ�ó���ID
	std::string m_app_id;
	// ��ѡ���ն˷�������ʱ�Ķ�λ��Ϣ��
	// ��ʽ�ǡ�γ��, ���ȡ���ע��˳�򣩣����磺39.983085, 116.80619��
	std::string m_gps;
	// ��ѡ ����vin�롣
	std::string m_vin;
	// ��ѡ �ն�mac��ַ��
	std::string m_mac;
	// ��ѡ���ն�������ַ��
	std::string m_bluetooth;
	// ��ѡ���û��ֻ����롣
	std::string m_tel;
	// ��ѡ�����ƺš�
	std::string m_carnum;
	// ��ѡ���������š�
	std::string m_engine;

	bool Init(const char *str);
};

class SessionParam
{
public:
	// 
	curl_slist* m_http_headers;
	// ������id
	int m_packet_id;
	// ��������ID
	int m_audio_index;
	// ������
	int m_error_code;
	// ������Ϣ	
	std::string m_error_message;

	ResultData *m_rd;
	// ��ѡ���ն˷�������ʱ�Ķ�λ��Ϣ��
	// ��ʽ�ǡ�γ��, ���ȡ���ע��˳�򣩣����磺39.983085, 116.80619��
	std::string m_gps;
	
	// ��ѡ��������Ƶ�ĸ�ʽ��
	// 1��8Khz��16bit��1channel
	// 2��16Khz��16bit��1channel��Ĭ�ϣ�
	std::string m_audio_format;
	
	//��ѡ����Ƶѹ���ȼ�����Χ��[0 - 10]��0��ʾ��ѹ����
	// Ĭ��10��
	// ֻ������������ʶ������ƶ�֮��Ľ�������Ӱ��ӿڳ������Ƶ���ݡ�
	std::string m_audio_quality;

	// ��ѡ���Ƿ񷵻������������ݡ�
	// 0�������أ�Ĭ�ϣ�
	// 1������
	std::string m_tts;

	// ��ѡ���������ӳ�ʱʱ�䣬��λ�롣Ĭ��3�롣
	std::string m_time_out;
	
	// ��ѡ����ǰҵ�����ͣ���poi������tel������music������radio��...��
	// Ӧ��ȷ����ǰҵ��ʱ������ͨ���ò���ָ�����Ա���߷�����ʶ��Ч�ʡ�
	std::string m_business;
	
	// ��ѡ����ǰ�������ơ� 
	std::string m_city;
	
	SessionParam(const char *str);
	~SessionParam();
};
