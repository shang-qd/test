#include "stdafx.h"
#include "VcyberVoiceSDK.h"
#include <curl/curl.h>
#include <json/json.h>
#include "Configs.h"
#include "SdkLib.h"

static Configs g_configs;
static CURL *curl = nullptr;

eInputReturnCode VInputInit(const char* configs) { 
	return VVSDK_ERR_WRONG_VERSION; 
}

eInputReturnCode VInputSessionBegin(const char* params, SESSION_HANDLE *handle) { 
	return VVSDK_ERR_WRONG_VERSION;
}

eInputReturnCode VInputPostData(SESSION_HANDLE handle, const void* waveData, unsigned int dataLen, int dataStatus, int* recogStatus) {
	return VVSDK_ERR_WRONG_VERSION;
}

eInputReturnCode VInputGetResult(SESSION_HANDLE handle, InputResultData **resultData) {
	return VVSDK_ERR_WRONG_VERSION;
}

eInputReturnCode VInputGetDetailError(SESSION_HANDLE handle, char **DetailError) {
	return VVSDK_ERR_WRONG_VERSION;
}

eInputReturnCode VInputEndSession(SESSION_HANDLE handle) {
	return VVSDK_ERR_WRONG_VERSION;
}

eInputReturnCode VInputFini() {
	return VVSDK_ERR_WRONG_VERSION;
}

eReturnCode CloudVDInit(const char * configs) {
	bool b_res = g_configs.Init(configs);
	if (b_res == false) {
		return CLOUDVD_ERR_INVALID_PARAM;
	}
	if (g_configs.m_server_addr.size() == 0	|| g_configs.m_app_id.size() == 0
		|| g_configs.m_vin.size() == 0 || g_configs.m_mac.size() == 0) {
		return CLOUDVD_ERR_INVALID_PARAM;
	}
	curl = curl_easy_init();
	if (curl == nullptr) {
		return CLOUDVD_ERR_SYSTEM_FAILED;
	}
	CURLcode res = curl_easy_setopt(curl, CURLOPT_URL, g_configs.m_server_addr.c_str());
	if (CURLE_OK == res){
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
		return CLOUDVD_SUCCESS;
	} else {
		// TODO 目前没有成功的都是系统错误（不在细分，用日志记录）
		return CLOUDVD_ERR_SYSTEM_FAILED;
	}
}

eReturnCode CloudVDStartSession(const char * params, SESSION_HANDLE * handle) {
	SessionParam *sp = new SessionParam(params);
	
	*handle = sp;

	sp->m_http_headers = curl_slist_append(nullptr, "Content-Type: application/json; charset=utf-8");
	sp->m_http_headers = curl_slist_append(sp->m_http_headers, "Expect: 100-continue");
	sp->m_http_headers = curl_slist_append(sp->m_http_headers, "Connection : Keep-Alive");

	CURLcode res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, sp->m_http_headers);
	std::string data = get_data(sp, &g_configs, "SessionBegin", nullptr, AUDIO_STATUS_CONTINUE);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

	std::vector<char*> vc;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &vc);
	res = curl_easy_perform(curl);
	
	Json::Value jsonobj = get_data(vc);
	sp->m_packet_id = jsonobj["packet_id"].asInt() + 1;
	
	int error_code = jsonobj["error_code"].asInt();
	std::string error_message = jsonobj["error_message"].asString();
	if (error_code > 0)
	{
		sp->m_error_code = error_code;
	}
	if (error_message.size() > 0)
	{
		sp->m_error_message = error_message;
	}

	if (CURLE_OK == res){
		return CLOUDVD_SUCCESS;
	} else {
		delete sp;
		*handle = nullptr;
		return CLOUDVD_ERR_SYSTEM_FAILED;
	}
}

eReturnCode CloudVDPostData(SESSION_HANDLE handle, const void * waveData, unsigned int dataLen, eAudioStatus dataStatus) {
	char *audio = new char[dataLen + 1];
	audio[dataLen] = '\0';
	memcpy(audio, waveData, dataLen);
	std::string data = get_data((SessionParam*)handle, &g_configs, "PutAudio", audio, dataStatus);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	std::vector<char*> vc;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &vc);
	CURLcode res = curl_easy_perform(curl);
	delete []audio;
	Json::Value jsonobj = get_data(vc);
	SessionParam *sp = (SessionParam *)handle;
	sp->m_packet_id = jsonobj["packet_id"].asInt() + 1;
	sp->m_business = jsonobj["business"].asString();

	int error_code = jsonobj["error_code"].asInt();
	std::string error_message = jsonobj["error_message"].asString();
	if (error_code > 0)
	{
		sp->m_error_code = error_code;
	}
	if (error_message.size() > 0)
	{
		sp->m_error_message = error_message;
	}

	if (!jsonobj["result"].isNull())
	{
		std::string out = jsonobj["result"].toStyledString();
		strcpy_s(sp->m_rd->result, 256, out.c_str());
		char sid[64] = {0};
		time_t lt = time(NULL);
		sprintf_s(sid, sizeof(sid), "%s%d", g_configs.m_vin.c_str(), lt);
		strcpy_s(sp->m_rd->uniqueSessionID, 64, sid);
	}
	if (!jsonobj["audio"].isNull())
	{
		std::string audio = jsonobj["audio"].asString();
		sp->m_rd->audioLenth = (int)audio.size();
		sp->m_rd->audio = (char*)realloc(sp->m_rd->audio, sp->m_rd->audioLenth + 1);
		sp->m_rd->audio[sp->m_rd->audioLenth] = '\0';
		memcpy(sp->m_rd->audio, audio.c_str(), sp->m_rd->audioLenth);
		char sid[64] = { 0 };
		time_t lt = time(NULL);
		sprintf_s(sid, sizeof(sid), "%s%d", g_configs.m_vin.c_str(), lt);
		strcpy_s(sp->m_rd->uniqueSessionID, 64, sid);
	}
	if (CURLE_OK == res){
		return CLOUDVD_SUCCESS;
	} else {
		return CLOUDVD_ERR_SYSTEM_FAILED;
	}
}

eReturnCode CloudVDGetResult(SESSION_HANDLE handle, ResultData ** resultData) {
	SessionParam *sp = (SessionParam*)handle;
	*resultData = sp->m_rd;
	return CLOUDVD_SUCCESS;
}

eReturnCode CloudVDSearchInfo(const char * params, SearchResult ** SearchResult) {
	return CLOUDVD_SUCCESS;
}

eReturnCode CloudVDEndSession(SESSION_HANDLE handle) {
	SessionParam *sp = (SessionParam*)handle;
	
	std::string data = get_data((SessionParam*)handle, &g_configs, "SessionEnd", "", AUDIO_STATUS_CONTINUE);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
	std::vector<char*> vc;
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &vc);
	CURLcode res = curl_easy_perform(curl);
	Json::Value jsonobj = get_data(vc);
	curl_slist_free_all(sp->m_http_headers);
	delete sp;
	if (CURLE_OK == res){
		return CLOUDVD_SUCCESS;
	}
	else {
		return CLOUDVD_ERR_SYSTEM_FAILED;
	}
	
	return CLOUDVD_SUCCESS;
}

eReturnCode CloudVDFini() {
	// 
	curl_easy_cleanup(curl);
	curl = nullptr;
	return CLOUDVD_SUCCESS;
}
