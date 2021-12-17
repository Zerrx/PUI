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
#include <limits> // For pausing the output
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\include\ShellControls.h"

void Pause(const bool DISPLAY_TEXT) {
	if (DISPLAY_TEXT)
		std::cout << "\n\nPress ENTER to continue...\n";
	else
		std::cout << std::endl;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
