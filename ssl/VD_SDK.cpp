// VD_SDK.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <curl/curl.h>

#pragma comment(lib, "ws2_32.lib")

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	printf("%s \n", (char*)ptr);
	return nmemb;
}

bool getUrl(char *filename)
{
	CURL *curl;
	CURLcode res;
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: Agent-007");
	curl = curl_easy_init();    // 初始化
	
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	//curl_easy_setopt(curl, CURLOPT_PROXY, "10.99.60.201:8080");// 代理
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);// 改协议头
	curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.133.1/Test.aspx");
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp); //将返回的http头输出到fp指向的文件
	//curl_easy_setopt(curl, CURLOPT_HEADERDATA, fp); //将返回的html主体数据输出到fp指向的文件

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	res = curl_easy_perform(curl);   // 执行
	if (res != 0) {
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
	}
	return true;
}


bool postUrl(char *filename)
{
	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();
	
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "1234"); 
	curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.133.1/Test.aspx");   
	// 指定url
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return true;
}

int main(int argc, char *argv[])
{
	CURL *curl;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "1234");
	curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.133.1/Test.aspx");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return 0;
}