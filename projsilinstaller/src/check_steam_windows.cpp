/*
* projsilinstaller-win/check_steam_windows.cpp
* purpose: Check if Steam and Half-Life are installed on both windows and linux systems (the filename is a lie).
*/


#if PLATFORM_WINDOWS
#include <conio.h>
#include <Windows.h>
#include <shlwapi.h>
#endif
#include <string>
#include "check_steam_windows.h"
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "shared_common_functions.h"


SharedCommonFuncs scfcsw; 

/// <summary>
/// purpose: Checks if the user has steam installed by checking the default installation path.
/// arguments: none
/// returns: Boolean true if steam is installed. 
/// </summary>
bool CheckSteamWindows::SteamDirectoryExists() {
	#if PLATFORM_WINDOWS
	const char* directory = "C:/Program Files (x86)/Steam"; //define the default steam directory
	#else
	const char* directory = "//.steam//debian-installation/"; //linux steam path
	#endif
	filesystem::path steamPath = filesystem::path(directory);
	if (filesystem::exists(steamPath)) {
		steamDirPathWin = directory;
		return true;
	}
	else {
		return false;
	}
}

/// <summary>
/// purpose: Checks if the user has Half-Life installed by checking the default installation path. Change the path for a different game when needed.
/// arguments: none
/// returns: Boolean true if Half-Life is installed.
/// </summary>
bool CheckSteamWindows::HalfLifeDirectoryExists() {

	const string hlDirectory = steamDirPathWin + "/steamapps/common/Half-Life"; //change game path here for different a different game
	filesystem::path halfLifePath = filesystem::path(hlDirectory);
	if (filesystem::exists(halfLifePath)) { //does the Half-Life directory exist?
		halflifeDirPathWin = hlDirectory;
		return true;
	} else { //it isnt installed
		return false;
	}
}

/// <summary>
/// purpose: returns steam path
/// arguments: none
/// returns: string steamDirPathWin
/// assignments: none
/// </summary>
string CheckSteamWindows::GetSteamDirectory() { return steamDirPathWin; }

/// <summary>
/// purpose: returns half-life path
/// arguments: none
/// returns: string halflifeDirPathWin
/// assignments: none
/// </summary>
string CheckSteamWindows::GetHalfLifeDirectory() { return halflifeDirPathWin; }

/// <summary>
/// purpose: allow the user to set the path manually if the auto detection fails
/// arguments: none
/// returns: none
/// assignments: user defined path to SteamDirPathWin
/// </summary>
void CheckSteamWindows::UserPathInput() {
	//clear screen
	scfcsw.ClearScreen();
	//ask user to input full path
	cout << "Input full path to the Steam folder:\n";
	string data;
	cin >> data;
	// C:\Program Files (x86)\Steam
	//switch backslashes
	//only for windows
	#if PLATFORM_WINDOWS
	std::replace(data.begin(), data.end(), '\\', '/');
	#endif
	//do convertions and
	//store data to steamDirPathWin
	steamDirPathWin = data.c_str();
}

/// <summary>
/// purpose: allow the user to set the path manually if the auto detection fails for Half-Life
/// arguments: none
/// returns: none
/// assignments: user defined path to halflifedirpathwin
/// 
/// </summary>
void CheckSteamWindows::UserPathHalfLifeInput() {
	//clear screen
	scfcsw.ClearScreen();
	//ask user to input full path
	cout << "Input full path to the folder where hl.exe is contained:";
	string data;
	cin >> data;
	// C:\Program Files (x86)\Steam
	//switch backslashes
	//only for windows
	#if PLATFORM_WINDOWS
	std::replace(data.begin(), data.end(), '\\', '/');
	#endif
	//do convertions and
	//store data to steamDirPathWin
	halflifeDirPathWin = data.c_str();


}

/// <summary>
/// purpose: sets the mod path on where to install the files
/// returns: full mod path 
/// </summary>
string CheckSteamWindows::ReturnFullModPath() {
	//string send = halflifeDirPathWin + "/projsil";
	return halflifeDirPathWin;
}

string CheckSteamWindows::Part() {
	string name = scfcsw.ColoredText(1, "[CheckSteamWindows]");
	return name;
}