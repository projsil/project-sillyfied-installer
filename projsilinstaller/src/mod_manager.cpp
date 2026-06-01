/*
* the entire mod/addon management part
*/

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include "json.h"
#include "mod_manager.h"
#include "shared_common_functions.h"
#include "update_package.h"


using json = nlohmann::json;
SharedCommonFuncs scfmm;
UpdatePackage upmm;
string modName = "unknown";
bool alreadyInstalled = false;
int versionReq = 0;
int modVer = 0;
string versionPath = "";
bool finalCheck = false;
string url = "";
string modVersionPath = "";

/// <summary>
/// Loads the data from the addon masterfile and stores it in a vector for easy access.
/// </summary>
/// <param name="file_path">Path where the masterfile is stored. (in temp)</param>
/// <returns></returns>
std::vector<json> load_addons(const std::string& file_path) {
	//get the file
	std::ifstream file(file_path);
	//check if it exists
	if (!file.is_open()) {
		std::cerr << "Error: Could not open the addon masterfile at " << file_path << "\n";
		return {};
	}

	//start actually parsing
	json j;
	file >> j;

	//loop through each addon and store it in the vector
	std::vector<json> addons;

	if (j.contains("addons") && j["addons"].is_object()) {
		for (const auto& item : j["addons"].items()) {
			addons.push_back(item.value()); //store the addon data
		}

	}
	return addons;
}

void ModManager::List(string _path) {
	scfmm.ClearScreen();
	cout << _path << "\n"; //debug
	cout << scfmm.ColoredBG(4, "Addon Browser - ") << scfmm.ColoredBG(4, _path) << "\n"; //blue
	cout << scfmm.ColoredBG(4, "ID - Name [version] - Category - Size - Req. Mod Ver. ") << "\n"; //blue

	try
	{
		auto addons = load_addons(_path);

		// iterate 
		for (size_t i = 0; i < addons.size(); ++i)
		{
			std::cout << i << ": " << addons[i].value("name", "<broken package>") << "[" << addons[i].value("addon-version", "<broken package>") << "] - " << addons[i].value("tag", "<broken package>") << " - " << addons[i].value("size", "<broken package>") << " - " << addons[i].value("required-mod-version", "<broken package>") << "\n";
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "error: " << e.what() << '\n';

	}

	std::cout << "End of list.\n";
	return;
}

string ModManager::ModView(string _choice, string _path) {
	// parse the input once and validate
	int index = -1;
	try {
		index = stoi(_choice);
	}
	catch (const std::exception& e) {
		std::cerr << "Input is not an int: " << e.what() << "\n";
		return string();
	}

	//load full properties of addon
	try
	{
		auto addons = load_addons(_path);
		if (index < 0 || static_cast<size_t>(index) >= addons.size()) {
			std::cerr << "Error: addon index out of range.\n";
			return string();
		}

		time_t uploadTime = static_cast<time_t>(addons[index].value("date-added", 0));
		cout << scfmm.ColoredBG(4, "Addon Details - ID: " + _choice) << "\n"; //blue
		cout << addons[index].value("name", "<broken package>") << " by " << addons[index].value("developer", "<broken package>") << "\n\n";
		cout << addons[index].value("description", "<broken package>") << "\n" <<
			"Credits to: " << addons[index].value("credits", "<broken package>") << "\n";
		cout << "Tag: " << addons[index].value("tag", "<broken package>") <<
			"\nAddon version: " << addons[index].value("addon-version", "<broken package>") <<
			"\nRequired mod version: " << addons[index].value("required-mod-version", "<broken package>") <<
			"\n\nDependencies and Conflicts:" <<
			"\nDeps: " << addons[index].value("dependencies", "<broken package>") <<
			"\nConflicts: " << addons[index].value("conflits-with", "<broken package>") <<
			"\n\nMore information:\n";


		char timeBuffer[26];
        struct tm* timeInfo = localtime(&uploadTime);
        if (timeInfo != nullptr && asctime_r(timeInfo, timeBuffer) != nullptr) {
            cout << "Size: " << addons[index].value("size", "<broken package>")
                << "\n"
                << "Time uploaded: " << timeBuffer
                << "License: " << addons[index].value("license", "<broken package>")
                << "\nAddon verified?: " << addons[index].value("add-on-verified", "<broken package>")
                << "\nHost URL: " << addons[index].value("download-url", "<broken package>")
                << "\n";
        }
        else {
            cout << "Size: " << addons[index].value("size", "<broken package>")
                << "\n"
                << "Time uploaded: <invalid time>"
                << "License: " << addons[index].value("license", "<broken package>")
                << "\nAddon verified?: " << addons[index].value("add-on-verified", "<broken package>")
                << "\nHost URL: " << addons[index].value("download-url", "<broken package>")
                << "\n";
        }
		//set the mod name for checking
		modName = addons[index].value("name", "unknown");
		versionReq = atoi(addons[index].value("required-mod-version", "unknown").c_str());
		modVer = atoi(addons[index].value("addon-version", "unknown").c_str());
		url = addons[index].value("download-url", "<broken package>");

	}
	catch (const std::exception& e)
	{
		std::cerr << "error: " << e.what() << '\n';
		// ensure we return a valid string on error
		return string();
	}

	// return the download URL (or empty string if none)
	return url;
}

void ModManager::CheckAlreadyInstalled(string _path) {
	//check if the addon is already installed
	std::ifstream file(_path + "/projsil/" + modName + ".SILADDONVER");
	versionPath = _path + "/projsil/" + modName + ".SILADDONVER";
	modVersionPath = _path + "/projsil/" + "version.projsil";
	if (file.is_open()) {
		cout << "Addon is already installed.\nWould you like to check for updates?\n";
		alreadyInstalled = true;
	}
	else {
		cout << "Addon is not installed.\nWould you like to install it?\n";
		alreadyInstalled = false;
	}
	cout << "(y/n): ";

}


void ModManager::Choice(string _path) {
	string choice;
	cin >> choice;
	char input = choice[0]; //get the letter
	switch (input) {
	case 'y':


	case 'Y':
		if (alreadyInstalled == true) {
			cout << "Checking for updates...\n";
			DoChecks(true, _path);
		}
		else {
			cout << "Installing addon...\n";
			DoChecks(false, _path);
		}
		break;
	case 'n':
		cout << "Returning to main menu...\n";
	}


}

void ModManager::DoChecks(bool alreadyInstalled, string _path) {
	//get the version file path

	
	//check if the addon is compatible with the mod version
	if (upmm.GetLatestVersion(modVersionPath) < versionReq) { //point to the MODs version file not the addons
		cout << Part() << scfmm.ColoredBG(1, "ERROR!") + "Mod is out of date. Please update the mod before installing this addon.\n";
		return;
	}
	else {
		cout << Part() << "Checked mod version.\n";
	}

	//do additional checks if already installed
	if (alreadyInstalled == true) {
		//check if the addon is up to date
		if (upmm.GetLatestVersion(versionPath) >= modVer) {
			cout << Part() << "Addon is already up to date. Returning to main menu...\n";
			finalCheck = false;
			return;
		}
		else {
			cout << Part() << "Updating addon from version " << upmm.GetLatestVersion(versionPath) << " to " << modVer << ". Installing...\n";
			finalCheck = true;
		}
	}
	else {
		cout << Part() << "All checks passed. Installing addon...\n";
		finalCheck = true;
	}

}

bool ModManager::ReturnStatus() {
	return finalCheck;
}

string ModManager::ReturnURL() {
	return url;
}

string ModManager::Part() {
	string name = scfmm.ColoredText(3, "[ModManager] ");
	return name;
}




