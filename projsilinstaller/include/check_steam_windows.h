using namespace std;
#pragma once

class CheckSteamWindows {
public:
	bool SteamDirectoryExists();
	bool HalfLifeDirectoryExists();
	string GetSteamDirectory();
	string GetHalfLifeDirectory();
	void UserPathInput();
	void UserPathHalfLifeInput();
	string ReturnFullModPath();
	string steamDirPathWin;
	string halflifeDirPathWin;
	string Part();
	
private:
};

