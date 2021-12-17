// Basic functions libraries
// System and Special includes
#include <cstdio>
#include <cstdlib> // For system processes such as system("pause");
#include <windows.h>
// Regular includes
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "include\conf.h"
#include "include\PUINamespace.h"
#include "include\ConfigurationFile.h"
#include "include\Interface.h"

// Standard namespace: std::<INFO>
// Application namespace: PUI::<INFO>

// Main function
int main(int argc, char* argv[]) {
	std::string tmp = (std::string)argv[0];
	PUI::applicationPath = tmp.substr(0, tmp.find_last_of("\\") + 1);
	
	// Before anything else...load the configuration information
	if (!PUI::config.read("PUI.cfg")) {
		std::cout << "\n\nSomething went wrong reading the configuration file...\n\n";
		
		// Return the value
		return 1;
	}
	
	// Keep application open until quitting
	while (true)
		if (!getInterface())
			break;
	
	// Return the value
	return 0;
}