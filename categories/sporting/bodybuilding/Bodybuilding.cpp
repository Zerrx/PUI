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
#include "..\..\..\include\Bodybuilding.h"
#include "..\..\..\include\CreateNewWeeklyDirectory.h"
#include "..\..\..\include\CreateShorthandRoutine.h"
#include "..\..\..\include\ShellControls.h"
#include "..\..\..\include\WindowsConsole.h"
#include "..\..\..\include\Interface.h"

using namespace std;

bool Bodybuilding() {
	// Clear the screen
	ClearScreen();
	
	// Get the introduction
	getIntroduction("bodybuilding");
	int bodybuildingInputHandlerContinue = bodybuildingInputHandler();
	if (bodybuildingInputHandlerContinue == 0) {
		std::cout << "\n\n\n\n\nClosing the application now...\n\n\n\n\n";
		return false;
	}
	return true;
}

int bodybuildingInputHandler(void) {
	std::string command;
	while (true) {
		setNewCommandLine();
		getline(std::cin, command);
		if (isClose(command))
			return 0;
		else if (isValidBodybuildingCommand(command))
			return executeBodybuildingCommand(command);
		else
			std::cout << "\nInvalid command or option is not available, please try again!";
	}
}

bool isValidBodybuildingCommand(std::string command) {
	// Initialize the isValidCommand bool
	bool isValidCommand = false;
	
	if ((command == "a")||
		(command == "b"))
		isValidCommand = true;

	return isValidCommand;
}

int executeBodybuildingCommand(std::string command) {
	// Initialize the quit variable
	int isNotQuit = 1;
	
	if (command == "a") { 
		// Create a new weekly directory command
		CreateNewWeeklyDirectory();
		isNotQuit = 1;
	} else if (command == "b") { 
		// Create a short-hand routine file
		CreateShorthandRoutine();
		isNotQuit = 1;
	}
	
	// If the check makes it here, quit the application...
	// Return the value
	return isNotQuit;
}


/* *********************************************************************
REMOVALS:
--------------
#include "..\..\..\lib\DateTime\DateTime.h"
#include "..\..\..\lib\File\FileIO.h"
#include "..\..\..\lib\Writing\Writing.h"
********************************************************************* */