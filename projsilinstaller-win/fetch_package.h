#include <curl/curl.h>
using namespace std;
#pragma once

class FetchPackage {
public:
	void StartFetchURL(string _url, string _fileName);
	string DownloadPackage(string _url, string _fileName);
	string GetTempDirectory();
	static size_t ShowProgress(void* ptr, size_t size, size_t nmemb, void* stream);
	string SendGlobalTempDir();
	static int ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
	string savedFilePath;
	string Part();
private:
	

};