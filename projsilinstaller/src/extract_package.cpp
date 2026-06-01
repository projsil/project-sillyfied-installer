#include <string>
#include <cstdlib>
#include "extract_package.h"
#ifdef _WIN64
#include <Windows.h>
#endif
#include <iostream>

using namespace std;
int Extract::ExtractArchive(string& archive, string& output_dir) {
#ifdef _WIN64
	//does 7zip exist?
	if (GetFileAttributesA("C:\\Program Files\\7-Zip\\7z.exe") == INVALID_FILE_ATTRIBUTES) {
		cerr << "Error! 7zip is not installed, cannot extract the file.\nPress any key to quit the application.";
		return 1738;
	}
	else {
		//7zip is installed, use it
		string cmd = "\"\"C:\\Program Files\\7-Zip\\7z.exe\" x \"" + archive + "\" -o\"" + output_dir + "\" -y\"";
		cout << "\nDebug: commandline structure" << cmd << "\n";
		return std::system(cmd.c_str());

	}
#else //most linux distros have 7z installed by default

	std::string cmd = "7z x \"" + archive + "\" -o\"" + output_dir + "\" -y";
	return std::system(cmd.c_str());
#endif
}