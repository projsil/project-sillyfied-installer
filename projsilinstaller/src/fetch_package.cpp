#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>     // for getenv
#include <cstdio>      // for tmpnam
#include <curl/curl.h>
#include "fetch_package.h"
#include <iomanip> // for std::setw
#include <chrono>
#include <cmath>       // for round
#include "shared_common_functions.h"

#ifdef _WIN64
#include <windows.h>
#include <shlobj.h> // for SHGetFolderPath
#endif

static auto last_time = chrono::steady_clock::now();
static curl_off_t last_dlnow = 0;
SharedCommonFuncs scffp;

int FetchPackage::ProgressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
	using namespace std::chrono;
	auto now = steady_clock::now();
	double mb_now = dlnow / (1024.0 * 1024.0);
	char loading_chars[] = { '|', '/', '-', '\\' };
	int index = (dlnow / 1048576) % 4; //every 2.5 mb
	char loading_char = loading_chars[index];



	last_time = now;
	last_dlnow = dlnow;

	if (dltotal > 0) {
		int percent = static_cast<int>((dlnow * 100) / dltotal);
		int mb_total = round(dltotal / (1024.0 * 1024.0));
		int progressBar = round(percent / 5);
		int notProgressBar = 20 - progressBar;


		std::cout << "\r" << loading_char << " Downloading: " << std::setw(3) << percent << "% - "
			<< std::fixed << std::setprecision(2)
			<< mb_now << " MB received out of " << mb_total << " MB - " << scffp.LoadingBar(progressBar, notProgressBar);
		std::cout.flush();
	}
	return 0;
}

void FetchPackage::StartFetchURL(string _url, string _fileName) {
	try {
		cout << Part() <<"Fetching package from " << _url << "\n";
		cout << Part() << "Preparing download...\n";
		//cin.get();
		string downloadedFile = DownloadPackage(_url, _fileName);
		cout << Part() << "\nFile downloaded to: " << downloadedFile << std::endl;
		savedFilePath = downloadedFile;

	}
	catch (const std::exception& e) {
		cerr << "Error: " << e.what() << std::endl;
	}

}

string FetchPackage::DownloadPackage(string _url, string _fileName) {
	string dir = GetTempDirectory();


	string fullFilePath = string(dir + _fileName); //
	savedFilePath = fullFilePath; //save the path externally from this function

	ofstream outFile(fullFilePath, ios::binary);
	if (!outFile) {
		throw runtime_error("ERROR! Failed to open temp file for writing. (FetchPackage::DownloadPackage)");
	}

	//curl stuff below respect button ------->>>
	//i hate myself

	CURL* curl = curl_easy_init(); //just init once easilly
	if (!curl) { //curl doesnt exist?
		throw runtime_error("ERROR! Couldn't init curl. (FetchPackage::DownloadPackage");
	}

	curl_easy_setopt(curl, CURLOPT_URL, _url.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FetchPackage::ShowProgress);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
	curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, FetchPackage::ProgressCallback);
	curl_easy_setopt(curl, CURLOPT_XFERINFODATA, nullptr); // or pass 'this' if needed
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L); // enable progress callback

	CURLcode result = curl_easy_perform(curl); //get results
	curl_easy_cleanup(curl); //clean everything up
	outFile.close(); //close the saved file

	if (result != CURLE_OK) { //did we mess up did we FUCK IT UP??
		throw runtime_error("ERROR! Download failed. Reason: " + string(curl_easy_strerror(result)));
	}
	return fullFilePath;
}

/// <summary>
/// gets the temp directory
/// platform: both
/// </summary>
/// <returns>temp directory path</returns>
string FetchPackage::GetTempDirectory() {
	//are we on windows?
#ifdef _WIN32
	char path[MAX_PATH];
	GetTempPathA(MAX_PATH, path); //windows feature to get the temp path
	return string(path);
#else //are we on linux or something else?
	return "/tmp/"; //place it in temp

#endif
}

size_t FetchPackage::ShowProgress(void* ptr, size_t size, size_t nmemb, void* stream) {
	std::ofstream* out = static_cast<std::ofstream*>(stream);
	size_t totalSize = size * nmemb;
	out->write(static_cast<char*>(ptr), totalSize);
	return totalSize;
	return 0;
}

string FetchPackage::SendGlobalTempDir() {
	return savedFilePath;
}

string FetchPackage::Part() {
	string name = scffp.ColoredText(2, "[FetchPackage] ");
	return name;
}