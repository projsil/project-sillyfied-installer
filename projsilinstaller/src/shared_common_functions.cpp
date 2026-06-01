#include <iostream>
#include "shared_common_functions.h"



/// <summary>
/// clears the screen, works on both platforms
/// </summary>
void SharedCommonFuncs::ClearScreen() {
	std::cout << "\033[2J\033[1;1H"; //i have no idea!
}

/// <summary>
/// colored text
/// </summary>
/// <param name="colorInt">[color number 0 to 6]  black red green yellow blue magenta cyan</param>
/// <param name="text">[text to color]</param>
string SharedCommonFuncs::ColoredText(int colorInt, string text) {
	//string of colors: black red green yellow blue magenta cyan
	string colors[7] = { "\033[1;30m", "\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m", };
	string combine = colors[colorInt] + text + "\033[0;37m"; //white is default
	return combine;
}

/// <summary>
/// colored background
/// </summary>
/// <param name="colorInt">[color number 0 to 6]  white red green yellow blue magenta cyan</param>
/// <param name="text">[text to put a colored background]</param>
string SharedCommonFuncs::ColoredBG(int colorInt, string text) {
	//string of colors: white red green yellow blue magenta cyan
	string colors[7] = { "\033[1;47m", "\033[1;41m", "\033[1;42m", "\033[1;43m", "\033[1;44m", "\033[1;45m", "\033[1;46m", };
	string combine = colors[colorInt] + text + "\033[0;40m"; //black is default
	return combine;
}

string SharedCommonFuncs::LoadingBar(int loaded, int notLoaded) {
	string bar = "[";
	for (int i = 0; i < loaded; i++) {
		bar += ColoredBG(4, "#");
	}
	for (int j = 0; j < notLoaded; j++) {
		bar += ColoredBG(0, "#");
	}
	bar += "]";
	return bar;
}

bool SharedCommonFuncs::IsNumber(const string& s) {
	for (char const& character : s) {
		if (std::isdigit(character) == 0) return false;
	}
	return true;
}