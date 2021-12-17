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
#include <dirent.h>
// Application includes
#include "..\..\include\FileSystem.h"

std::vector<std::string> getDirectoryListing(std::string SPECIFIED_DIRECTORY) {
	std::vector<std::string> files;
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(SPECIFIED_DIRECTORY.c_str())) != NULL) {
		// Do not print bad output (i.e. "." and "..")
		int stopBadOutput = 0;
		// List all the files and directories within directory
		while ((ent = readdir(dir)) != NULL) {
			if (!(stopBadOutput >= 2)) {
				stopBadOutput++;
				continue;
			}
			files.push_back(ent->d_name);
		}
		closedir(dir);
	} else {
		// Could not open directory
		perror("");
		// Push failed string
		files.push_back("-1 FAILED -1");
	}
	
	// Return the value
	return files;
}

void copyFilesInDirectory(std::string FROM_DIRECTORY,
		std::string TO_DIRECTORY,
		std::vector<std::string> FILES) {
	// Set the incrementing variable to iterate through the list
	int i = 0;
	int sizeOfFILES = FILES.size();
	while (i < sizeOfFILES) {
		// Copy the files
		CopyFile((FROM_DIRECTORY + FILES[i]).c_str(),
				(TO_DIRECTORY + FILES[i]).c_str(),
				true);
		i++;
	}
	// End the function
}