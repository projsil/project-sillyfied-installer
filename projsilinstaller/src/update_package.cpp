#include "update_package.h"
#include "shared_common_functions.h"
#include <iostream>
#include <fstream>

using namespace std;
SharedCommonFuncs scfup; //shared common functions used in both OS's

bool UpdatePackage::Compare(string tempPath, string gamePath) {
  gamePath += "\\projsil\\version.projsil"; //append the file name
  cout << Part() << "Checking for new versions...\n";
  //read both files and convert string to int
  ifstream tf(tempPath);
  ifstream gf(gamePath);
  int tempVersion, gameVersion = 0;
  tf >> tempVersion;
  gf >> gameVersion;
  //compare, highest is newest
  if (tempVersion > gameVersion) {
    cout << Part() << "Updating mod from " << gameVersion << " to " << tempVersion << ". Starting download...\n";
    return false; //needs update
  } else {
    cout << Part() << "Mod is already on the latest version. Returning... (you have: " << gameVersion << " - recent version is: " << tempVersion << ".\n";
    return true; //no update needed
  }

}

int UpdatePackage::GetLatestVersion(string _path) {
  ifstream tf(_path);
  int tempVersion = 0;
  tf >> tempVersion;
  return tempVersion;
}

string UpdatePackage::Part() {
  string name = scfup.ColoredText(6, "[UpdatePackage]");
  return name;
}