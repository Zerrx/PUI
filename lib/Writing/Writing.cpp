// Function Prototypes
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
#include "..\..\include\Writing.h"
#include "..\..\include\ShellControls.h"
#include "..\..\include\WindowsConsole.h"
#include "..\..\include\DateTime.h"
#include "..\..\include\FileIO.h"
#include "..\..\include\FileSystem.h"
#include "..\..\include\Interface.h"

int Writing() {
	// Clear the screen
	ClearScreen();
	
	// Get the introduction
	getWritingIntroduction();
	
	// TESTING
	Pause(true);
	
	// Return the value
	return 0;
}

void getWritingIntroduction() {
	std::cout << "****************************************************************\n";
	std::cout << "Welcome to the Writing application! This application allows a\n";
	std::cout << "user to write TXT files for TXT applications or exporting to DOC\n";
	std::cout << "applications. The user has the following formatting options for\n";
	std::cout << "the creation of the file:\n";
	std::cout << std::endl;
	std::cout << "a. Double-Spaced, Ex.\t\tb. Paragraph-Space, Ex.\n";
	std::cout << "\tThis is a Paragrah.\t\t\tThis is a paragraph.\n";
	std::cout << "\t\t\t\t\tThis is line 2. This\n";
	std::cout << "This is line 2. This\t\tis line 3.\n";
	std::cout << "\n";
	std::cout << "is line 3.\t\t\t\tThis is a new\n";
	std::cout << "\t\t\t\t\tparagraph.\n";
	std::cout << "\n";
	std::cout << "\n";
	std::cout << "****************************************************************\n";
}