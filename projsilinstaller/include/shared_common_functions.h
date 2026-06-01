using namespace std;
#pragma once

class SharedCommonFuncs {
public:
	void ClearScreen();
	string ColoredText(int colorInt, string text);
	string ColoredBG(int colorInt, string text);
	string LoadingBar(int loaded, int notLoaded);
	bool IsNumber(const string& s);
private:
};

