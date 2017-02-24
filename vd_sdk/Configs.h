#pragma once

#include <string>
#include <curl/curl.h>

#include "VcyberVoiceSDK.h"

class Configs {
public:
	// 必选 服务器地址
	std::string m_server_addr;
	// 必选 应用程序ID
	std::string m_app_id;
	// 可选。终端发起请求时的定位信息。
	// 格式是“纬度, 经度”（注意顺序），例如：39.983085, 116.80619。
	std::string m_gps;
	// 必选 车辆vin码。
	std::string m_vin;
	// 必选 终端mac地址。
	std::string m_mac;
	// 可选。终端蓝牙地址。
	std::string m_bluetooth;
	// 可选。用户手机号码。
	std::string m_tel;
	// 可选。车牌号。
	std::string m_carnum;
	// 可选。发动机号。
	std::string m_engine;

	bool Init(const char *str);
};

class SessionParam
{
public:
	// 
	curl_slist* m_http_headers;
	// 包数据id
	int m_packet_id;
	// 语音数据ID
	int m_audio_index;
	// 错误码
	int m_error_code;
	// 错误消息	
	std::string m_error_message;

	ResultData *m_rd;
	// 可选。终端发起请求时的定位信息。
	// 格式是“纬度, 经度”（注意顺序），例如：39.983085, 116.80619。
	std::string m_gps;
	
	// 可选。发送音频的格式。
	// 1：8Khz，16bit，1channel
	// 2：16Khz，16bit，1channel（默认）
	std::string m_audio_format;
	
	//可选。音频压缩等级。范围：[0 - 10]。0表示不压缩。
	// 默认10。
	// 只用来控制语音识别库与云端之间的交互，不影响接口出入的音频数据。
	std::string m_audio_quality;

	// 可选。是否返回语音播报数据。
	// 0：不返回（默认）
	// 1：返回
	std::string m_tts;

	// 可选。网络连接超时时间，单位秒。默认3秒。
	std::string m_time_out;
	
	// 可选。当前业务类型：“poi”、“tel”，“music”、“radio”...。
	// 应用确定当前业务时，可以通过该参数指定，以便提高服务器识别效率。
	std::string m_business;
	
	// 可选。当前城市名称。 
	std::string m_city;
	
	SessionParam(const char *str);
	~SessionParam();
};
