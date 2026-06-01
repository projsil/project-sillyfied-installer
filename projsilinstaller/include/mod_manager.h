#include <string>
using namespace std;
#pragma once

class ModManager {
public:
	void List(string _path);
	string ModView(string _choice, string _path);
	void CheckAlreadyInstalled(string _path);
	void Choice(string _path);
	void DoChecks(bool alreadyInstalled, string _path);
	bool ReturnStatus();
	string ReturnURL();
	string Part();


private:
	
};
