// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
// Regular includes
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\include\Construction.h"
#include "..\..\include\StairStringer.h"
#include "..\..\include\ShellControls.h"
#include "..\..\include\WindowsConsole.h"
#include "..\..\include\Interface.h"

using namespace std;
// TODO: this test in construction.cpp
bool Construction() {
	// Clear the screen
	ClearScreen();
	// Get the introduction
	getIntroduction("construction");
	int constructionInputHandlerContinue = constructionInputHandler();
	if (constructionInputHandlerContinue == 0) {
		std::cout << "\n\n\n\n\nClosing the application now...\n\n\n\n\n";
		return false;
	}
	return true;
}

int constructionInputHandler(void) {
	std::string command;
	while (true) {
		setNewCommandLine();
		getline(std::cin, command);
		if (isClose(command))
			return 0;
		else if (isValidConstructionCommand(command))
			return executeConstructionCommand(command);
		else
			std::cout << "\nInvalid command or option is not available, please try again!";
	}
}

bool isValidConstructionCommand(std::string command) {
	// Initialize the isValidCommand bool
	bool isValidCommand = false;
	
	if ((command == "a")||
		(command == "b"))
		isValidCommand = true;

	return isValidCommand;
}

int executeConstructionCommand(std::string command) {
	// Initialize the quit variable
	int isNotQuit = 1;
	
	if (command == "a") { 
		// Create a new weekly directory command
		StairStringer();
		isNotQuit = 1;
	}
	
	// If the check makes it here, quit the application...
	// Return the value
	return isNotQuit;
}
