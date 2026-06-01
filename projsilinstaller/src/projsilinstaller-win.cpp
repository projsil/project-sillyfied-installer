//this is a crossplatform installer, implement it first for windows then linux
#include <iostream>
#include <ranges>
#include <algorithm>
#include "platform.h"
#if PLATFORM_WINDOWS
#include <Windows.h> 
#include <conio.h>
#endif
#include "projsilinstaller.h"
#include "check_steam_windows.h"
#include "shared_common_functions.h"
#include "fetch_package.h"
#include "extract_package.h"
#include "update_package.h"
#include "remove_enitre_game.h"
#include "mod_manager.h"



using namespace std;

//load every class






//linux classes

//both dependent classes
ProjSilInstaller psi;
CheckSteamWindows csw; //check steam for both os's even tho the class has windows in the name ignore that
SharedCommonFuncs scf; //shared common functions used in both OS's
FetchPackage fp; //fetchpackage used in both os's
Extract ex; //extract package used in both os's
UpdatePackage up; //update package used in both os's
RemoveGame rg;
ModManager mm;

//set variables
string directoryWin; //steam directory
string hlDirWin; //half-life directory
string modDirWin; //mod directory
string downloadedFilePath; //path to the downloaded file
bool quit = false;


int main()
{
	//do some linux checks here
	psi.CheckLinuxProperities();
	//prepare the installer and get the location
	cout << "Finding Steam...\n";
	if (csw.SteamDirectoryExists()) {
		cout << psi.Part() << "Steam found! Locating game files...\n";
	}
	else {
		cout << psi.Part() << scf.ColoredBG(1, "ERROR!") + " Could not find Steam. This might be because:\n- You do not have Steam installed.\n- Steam is installed on an external drive.\n- Or an internal error accured.\n\nWould you like to input the path manually? (y/n default)\n";
		//insert choice here
		string data;
		cin >> data;
		//if choice is y
		if (data == "y") {
			//call userpathinput
			csw.UserPathInput();
		}
		else if (data == "n") {
			exit(0);
		}
		else {
			cout << "Invalid input, closing program by default... Please press any key.\n";
			cin.get();
			exit(0);
		}
		///end if else statement
	}

	cout << "Searching for the Half-Life directory\n";
	if (csw.HalfLifeDirectoryExists()) {
		cout << psi.Part() << "Half-Life found! Continuing to main menu...\n";
	}
	else {
		cout << "Error! Could not find Half-Life! If you have it installed; would you like to input the path manually? (y/n (default))\n";
		string data;
		cin >> data;
		if (data == "y") {
			//call manualpathinputwindows
			csw.UserPathHalfLifeInput();
		}
		else if (data == "n") {
			exit(0);
		}
		else {
			cout << psi.Part() << "Invalid input, closing program by default... Please press any key.\n";
			cin.get();// universal console readline
			exit(0);
		}
		///end if else statemen

	}

	directoryWin = csw.GetSteamDirectory(); //get directory
	cout << psi.Part() << "Steam path: " + directoryWin + ".\n";
	hlDirWin = csw.GetHalfLifeDirectory();
	cout << psi.Part() << "Half-Life 1 path: " + hlDirWin + ".\n";
	cout << psi.Part() << "Press any key to continue.\n";
	cin.get();
	psi.AssignModPath();
	psi.MainMenu();
}

void ProjSilInstaller::AssignModPath() {
	modDirWin = csw.ReturnFullModPath();
}

//todo clean up and put choices in funcs
void ProjSilInstaller::MainMenu() {
	do {
		PrintMenuText();
		string choice;
		cin >> choice;
		char input = choice[0]; //get the letter


		//okay! we cannot use strings with switches! cool! umm awesome!
		switch (input) {
		case 'i':
		case 'I':
		case '1':
			Install();
			break;
		case 'u':
		case 'U':
		case '2':
			Update();
			break;
		case 'r':
		case 'R':
		case '3':
			Repair();
			break;
		case 'A':
		case 'a':
		case '4': {
			Addons();
			break;
		}
		case 'P':
		case 'p':
		case '5':
			Path();
			break;
		case 'N':
		case 'n':
		case '6':
			Uninstall();
			break;
		case 'Q':
		case 'q':
		case '7':
			Quit();
			break;
		}

	} while (quit == false);
}

void ProjSilInstaller::PrintMenuText() {
	cout << "\n";
	cout << scf.ColoredBG(4, "I") + "nstall - Install Project Sillyfied to the Half-Life directory.\n";
	cout << scf.ColoredBG(4, "U") + "pdate - Update the mod to the latest version.\n";
	cout << scf.ColoredBG(4, "R") + "epair - Repairs the mod files by reinstalling.\n";
	cout << scf.ColoredBG(4, "A") + "ddons - Browse addons and install them to the mod.\n";
	//cout << scf.ColoredBG(4, "C") + "lean - Clean up temp files to save space.\n"; //unused since its saved to the os's temp folder
	cout << scf.ColoredBG(4, "P") + "ath - Change the Half-Life game path if the installer messed up automatically assigning.\n";
	cout << "U" + scf.ColoredBG(4, "n") + "install - Remove Project Sillyfied from your Half-Life directory.\n";
	cout << scf.ColoredBG(4, "Q") + "uit - Quit the installer.\n";
	cout << "\n";
	cout << "Current installation path: " + modDirWin + "\n";
	cout << "----------------------------------------------------------------\n";
	cout << "Input highlighted character:";
}

void ProjSilInstaller::Install() {
	fp.StartFetchURL("https://sillyfications.github.io/projsil/projsil.7z", "projsil_package.7z"); //
	downloadedFilePath = fp.SendGlobalTempDir();
	cout << fp.Part() << "Calling 7zip...\n";
	if (ex.ExtractArchive(fp.savedFilePath, modDirWin) == 1738) {
		cin.get();
		exit(0);
	}
	else {
		cout << fp.Part()<< "Project Sillyfied has successfully installed!\nReturning to main menu...\n\n\n";
	}
	//checkdirectoryempty 
}
void ProjSilInstaller::Update() {
	fp.StartFetchURL("https://sillyfications.github.io/projsil/version.projsil", "version.projsil"); //get the version file online
	downloadedFilePath = fp.SendGlobalTempDir();

	if (up.Compare(fp.savedFilePath, modDirWin) == false) {
		//needs update
		fp.StartFetchURL("https://sillyfications.github.io/projsil/projsil.7z", "projsil_package.7z"); //
		downloadedFilePath = fp.SendGlobalTempDir();
		cout << up.Part()<< "Calling 7zip...\n";
		if (ex.ExtractArchive(fp.savedFilePath, modDirWin) == 1738) {
			cin.get();
			exit(0);
		}
		else {
			cout << up.Part()<< "Project Sillyfied has successfully updated!\nReturning to main menu...\n\n\n";
		}
	}
	else {
		//no update needed
	}

}
void ProjSilInstaller::Repair() {
	cout << rg.Part()<< "Are you sure you want to repair the mod?\n\nThis will remove all SAVE files and installed addons. If you want to keep both of it, please make a backup of the SAVE folder and the addons file (addons.projsil) Do you want to continue?\n(y/n default) ";
	if (rg.Confirmation()) {
		rg.Remove(modDirWin);
	}
	fp.StartFetchURL("https://sillyfications.github.io/projsil/projsil.7z", "projsil_package.7z"); //
	downloadedFilePath = fp.SendGlobalTempDir();
	cout << fp.Part()<< "Calling 7zip...\n";
	if (ex.ExtractArchive(fp.savedFilePath, modDirWin) == 1738) {
		cin.get();
		exit(0);
	}
	else {
		cout << fp.Part()<< "Project Sillyfied has been successfully repaired.\nReturning to main menu...\n\n\n";
	}

}
void ProjSilInstaller::Addons() {
	fp.StartFetchURL("https://sillyfications.github.io/projsil/addons.json", "projsil.masteraddon"); //
	downloadedFilePath = fp.SendGlobalTempDir();
	mm.List(downloadedFilePath);
	cout << mm.Part()<< "Input addon ID to show more information about the addon.\n";
	string data;
	cin >> data;
	string url = mm.ModView(data, downloadedFilePath);
	mm.CheckAlreadyInstalled(modDirWin);
	mm.Choice(modDirWin);
	if (mm.ReturnStatus() == true) {
		fp.StartFetchURL(mm.ReturnURL(), "projsil_package.7z");
		downloadedFilePath = fp.SendGlobalTempDir();
		cout << fp.Part()<< "Calling 7zip...\n";
		if (ex.ExtractArchive(fp.savedFilePath, modDirWin) == 1738) {
			cin.get();
			exit(0);
		}
		else {
			cout << mm.Part()<< "Addon has been successfully installed/updated!\nReturning to main menu...\n\n\n";
		}
	}
	else {
		cout << mm.Part()<< "Returning to main menu...\n";
	}


}
void ProjSilInstaller::Path() {
	csw.UserPathInput();
	csw.UserPathHalfLifeInput();
	AssignModPath();

}
void ProjSilInstaller::Uninstall() {
	cout << rg.Part()<< "Are you sure?\n\nRemoving the entire game will require you to reinstall the game and the addons you downloaded! If you are sure, make sure you made BACK-UPS of the addon master file (addons.projsil) and your saves in the SAVE folder. or not if you don't care whatever rows your boat idk.\n(y/n default) ";
	if (rg.Confirmation()) {
		rg.Remove(modDirWin);
	}

}
void ProjSilInstaller::Quit() {
	quit = true;
}

void ProjSilInstaller::CheckLinuxProperities() {
#ifdef PLATFORM_LINUX
	cout << Part() << "Running Linux checks...\n";
	//check if we are running as sudo
	//continue if we are because we need permissions to write to steam folder
	if (getuid() != 0) {
		cout << Part()<< scf.ColoredBG(1, "ERROR!") + " Please run the installer as root (using sudo) to ensure proper permissions to write to the Steam folder.\n";
		cout << Part()<< "Press any key to exit.\n";
		cin.get();
		exit(0);
	}
	else {
		cout << Part()<< "Sudo permissions detected, continuing...\n";
	}

	//check if we are in the home directory
	char* homeDir = getenv("HOME");
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
	string currentDir(cwd);
	string homeDirStr(homeDir);
	if (currentDir != homeDirStr) {
		cout << Part()<< scf.ColoredBG(3, "WARNING!") + " It is recommended to run the installer from your home directory to avoid write issues. Continue anyway? (y/n default)\n";
		string data;
		cin >> data;
		if (data == "y") {
			cout << Part()<< "Continuing...\n";
		}
		else {
			cout << Part()<< "Exiting...\n";
			cin.get();
			exit(0);
		}
	}
	else {
		cout << Part()<< "Home directory detected, continuing...\n";
	}


	#else
	cout << Part()<< "Skipping Linux checks; user is using Windows.\n";
	#endif
}

string ProjSilInstaller::Part() {
	string name = scf.ColoredText(4, "[ProjSilInstaller] ");
	return name;
}



