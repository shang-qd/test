// vd_sdk.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "stdafx.h"
#include <curl/curl.h>
#include <json/json.h>

static CURL *curl;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	//int written = fwrite(ptr, size, nmemb, (FILE *)fp);
	printf("%s \n",ptr);
	return size * nmemb;
}

bool getUrl()
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();    // ��ʼ��
	if (curl)
	{
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// ����
		//curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// ��Э��ͷ
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_URL, "https://127.0.0.1");
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //�����ص�httpͷ�����fpָ����ļ�
		//curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //�����ص�html�������������fpָ����ļ�
		res = curl_easy_perform(curl);   // ִ��
		if (res != 0) {
			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}
	}
	return true;
}

static bool postUrl()
{
	CURLcode res;
	
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_WRITEDATA,curl);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, FALSE);
		//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "/tmp/cookie.txt"); // ָ��cookie�ļ�
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "1234");    // ָ��post����
		//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");
		curl_easy_setopt(curl, CURLOPT_URL, "https://127.0.0.1/Test.aspx");
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	return true;
}

int main_abc(void)
{
	Json::Value jsonobj;
	Json::Reader jsonreader;
	char *str = "{\"server_addr\":\"10.1.8.110:555\",\"app_id\":\"CYW00021\",\"vin\":\"1234444\",\"mac\":\"123123123123\"}";

	jsonreader.parse(str, jsonobj, false);
	printf("%s \n", jsonobj["server_addr"].asString().c_str());

	// SDK�����׽ӿڣ�һ�����������뷨��һ����VD
	// getUrl();
	// ������ϣ��ֻ��һ��
	postUrl();
	return 0;
}