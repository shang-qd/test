#include "stdafx.h"
#include "Configs.h"
#include <json/json.h>


bool Configs::Init(const char *str)
{
	Json::Value jsonobj;
	Json::Reader jsonreader;

	bool res = jsonreader.parse(str, jsonobj, false);
	if (res == false) {
		return false;
	}
	m_server_addr = jsonobj["server_addr"].asString();
	m_app_id = jsonobj["app_id"].asString();
	m_gps = jsonobj["gps"].asString();
	m_vin = jsonobj["vin"].asString();
	m_mac = jsonobj["mac"].asString();
	m_bluetooth = jsonobj["bluetooth"].asString();
	m_tel = jsonobj["tel"].asString();
	m_carnum = jsonobj["carnum"].asString();
	m_engine = jsonobj["engine"].asString();
	return true;
}

SessionParam::SessionParam(const char *str)
{
	m_rd = new ResultData;
	m_rd->audio = nullptr;
	m_rd->audioLenth = 0;
	m_rd->result = new char[256];
	memset(m_rd->result,'\0',256);
	m_rd->uniqueSessionID = new char[64];
	memset(m_rd->uniqueSessionID, '\0', 64);

	Json::Value jsonobj;
	Json::Reader jsonreader;

	bool res = jsonreader.parse(str, jsonobj, false);
	if (res == false) {
		throw "param error";
	}
	m_packet_id = 1;
	m_audio_index = 0;
	m_error_code = 0;
	m_gps = jsonobj["gps"].asString();
	m_audio_format = jsonobj["audio_format"].asString();
	m_audio_quality = jsonobj["audio_quality"].asString();
	m_tts = jsonobj["tts"].asString();
	m_time_out = jsonobj["time_out"].asString();
	m_business = jsonobj["business"].asString();
	m_city = jsonobj["city"].asString();
}

SessionParam::~SessionParam()
{
	if (m_rd->audio != nullptr)
	{
		free(m_rd->audio);
	}
	delete []m_rd->result;
	delete []m_rd->uniqueSessionID;
}