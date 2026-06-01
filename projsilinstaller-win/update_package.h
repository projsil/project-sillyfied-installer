#include <string>
using namespace std;
#pragma once

class UpdatePackage {
public:
	bool Compare(string tempPath, string gamePath);
	int GetLatestVersion(string _path);
	string Part();
private:
}; 
