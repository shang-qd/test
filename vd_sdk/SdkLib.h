#pragma once

#include "VcyberVoiceSDK.h"
#include <curl/curl.h>
#include <json/json.h>
#include "Configs.h"

std::string GBKToUTF8(const char* strGBK);
std::string UTF8ToGBK(const char* strUTF8);

Json::Value get_data(std::vector<char*> &vc);
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);
std::string get_data(SessionParam *sp, Configs *config, const char* method, const char* audio, eAudioStatus dataStatus);
