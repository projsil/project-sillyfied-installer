#include "remove_enitre_game.h"
#include "shared_common_functions.h"
#include <iostream>
//isnt really supported on C++ 14
//#include <filesystem> // Add this include
using namespace std;

SharedCommonFuncs scfrg; //shared common functions used in both OS's
void RemoveGame::Remove(string gamePath) {
	//just call the command line for each os
#ifdef PLATFORM_WINDOWS // windows terminal command
	string command = "rmdir /S \"" + gamePath + "\\projsil\"";
	system(command.c_str());

#else // linux terminal command
	string command = "rm -rf '" + gamePath + "/projsil'";
	system(command.c_str());

#endif
	cout << Part() << "Removed the game files.";
}

bool RemoveGame::Confirmation() {
	char choice;
	string temp;
	cin >> temp;
	choice = temp[0];
	if (choice == 'y' || choice == 'Y') {
		return true;
	}
	else {
		return false;
	}
}

string RemoveGame::Part() {
	string name = scfrg.ColoredText(5, "[RemoveGame]");
	return name;
}