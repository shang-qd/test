#include "stdafx.h"
#include "SdkLib.h"
#include <vector>

static char* user_info = "5413721411a020575107309077512060b010801767746544c13000804747b15730608060207607077000d6d7817740e0578070572434245264654580a0170235d3d1b54264";

std::string get_data(SessionParam *sp, Configs *config, const char* method,
	const char* audio, eAudioStatus dataStatus)
{
	//{ "packet_id":3, 
	//"method" : "PutAudio", 
	// "business" : "", 
	// "audio_index" : 0, 
	// "audio_last" : 0, 
	// "audio" : "LgDrn09g8sT0X5oAYFrFtBeo9d7NPHT7Y1cHSjT3XpKjdjwfruihtC4Sj9/iUYb/NgDtXaBJOvIpTj1GMXT0ZccaYXypWEAfqOVm3NFV+JW0acgpQmMafz7b8HkLiSGTFwN7bkH1n6tgANKCwz7txaQTGCo91FfS4XBM8j4z5Z0KAsb01S1UubTSFi4PZ6tuZEsRyiNWFjlAmyiDfml1StARhUfcVhaPm3YGRLgSLN3AMPQjd2JxTVZCGv4p4n06dJOauIdu5JA0714A2NgXkJuZPFekyC/6NMEUNbSB9zinPLrZow8TBeUMSOp/fxmsrMhxPBYZTkwRO0jkewP68vBIMGmgt16mJ8f1xDwEwqw73RhgEcctkf4VsYnh67trj9d143dFiyYkH1QAz00aq436fxyqF6gMTlBtwtqVbBVmzOVN0NTM2xxmkjwKDZQG0VVRMqLZZrdt4GKJaLx6nngaz1d0/7xCrDXrZJwyFNoUaNx9kl244FrWbezPd717TADQFrqjPlEm6yCftDnlytvz9BvKvvma1jZdjLHcnVu6RUjQF7kd8LxwiYkgFPT993HwzNhXDkJIahEymhVIAN6rua1XLsGo2q7detpHWQDPP7PBaJM/L8d11wH7yuBTB+E9O3fQFtn/1JIPC9SZLNTSTTkfwsiN2fOirX4RVpq4m1oUz6Ly6y3RVgt1pZt+EOdBOYahOM6H8MyyMU1P/goDmKkPl58JP2QAyjo3GDMm/bmvCSyueuydFD8Uz3UlIzI9J8FKLWEX6jcQG67qXfbb0VOZUmVjPn9lquHamvZALL9sKlWUA4BGCQ1VuzYqmx/JYbGDLq7ZBNFcT3eXra6fjqRVslqxYyrfDTGnvw==" }

	Json::Value data;
	data["packet_id"] = Json::Value(sp->m_packet_id);
	data["method"] = Json::Value(method);
	data["business"] = Json::Value(sp->m_business);
	if (strcmp("SessionBegin", method) == 0)
	{
		data["app_id"] = Json::Value(config->m_app_id);
		data["user_info"] = Json::Value(user_info);
		data["tts"] = Json::Value(sp->m_tts);
		data["audio_format"] = Json::Value(sp->m_audio_format);
		data["audio_quality"] = Json::Value(sp->m_audio_quality);
	}
	else if (strcmp("PutAudio", method) == 0)
	{
		if (AUDIO_STATUS_FIRST == dataStatus)
		{
			sp->m_audio_index = 0;
		}
		data["audio_index"] = Json::Value(sp->m_audio_index);
		if (AUDIO_STATUS_LAST == dataStatus)
		{
			data["audio_last"] = Json::Value(1);
		}
		else
		{
			data["audio_last"] = Json::Value(0);
		}
		sp->m_audio_index++;
		data["audio"] = Json::Value(audio);
	}
	else if (strcmp("SessionEnd", method) == 0)
	{
		data["business"] = Json::Value("");
		data["error_code"] = Json::Value(sp->m_error_code);
		data["error_message"] = Json::Value(sp->m_error_message);
	}
	Json::FastWriter writer;
	std::string str_data = writer.write(data);
	return str_data;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::vector<char*>* vc = (std::vector<char*>*)stream;
	size_t stDataLen = size * nmemb;
	char* str = new char[stDataLen + 1];
	str[stDataLen] = '\0';
	memcpy(str, ptr, stDataLen);
	vc->push_back(str);
	return size * nmemb;
}

Json::Value get_data(std::vector<char*> &vc)
{
	std::string res = "";
	for (int i = 0; i < vc.size(); i++)
	{
		res += vc[i];
	}
	
	Json::Value jsonobj;
	Json::Reader jsonreader;
	jsonreader.parse(res.c_str(), jsonobj, false);

	for (int i = 0; i < vc.size(); i++)
	{
		delete vc[i];
	}
	vc.clear();
	return jsonobj;
}

std::string GBKToUTF8(const char* strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, strGBK, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, strGBK, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	std::string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

std::string UTF8ToGBK(const char* strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}
