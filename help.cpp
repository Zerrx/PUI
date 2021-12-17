// Regular includes
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
// Application includes
#include "include\FileIO.h"
#include "include\ShellControls.h"

using namespace std;

// Function Prototypes
void displayShorthandHelpMenu(int argc,
		std::vector<std::string> argv);

bool isMatchingString(std::string haystack,
		std::string needle);

std::vector<std::string> RemoveDuplicateEntries(
		std::vector<std::string> entries);


// Main function
int main(int argc, char* argv[]) {
	// Need the arguments after (if coming from the terminal)
	// the path to the program
	std::vector<std::string> argList;
	int argListSize = 0;
	
	// Determine whether arguments came from the terminal
	// or a program by checking for the path to the program
	if ((isMatchingString((std::string)argv[0],
			"C:\\Env\\Dev\\PUI\\app\\PUI\\help.exe")) ||
		(isMatchingString((std::string)argv[0],
			"help.exe")))
		argListSize = 1;

	for (argListSize; argListSize < argc; argListSize++)
		argList.push_back((std::string)argv[argListSize]);
	
	// After getting parameters for the program,
	// ensure the argument count reflects parsed
	// data count (-program_info)
	argListSize = argList.size();
	
	// Check for the help menu to use and use it
	for (int i = 0; i < argListSize; i++) {
		if (argList[i] == "<deleteDuplicates>") {
			std::ifstream dupFile;
			dupFile.open("cpp.files");
			std::vector<std::string> items;
			std::string item;
			while(getline(dupFile, item))
				items.push_back(item);
			dupFile.close();
			items = RemoveDuplicateEntries(items);
			std::ofstream oFile;
			oFile.open("cpp.files");
			for (int z = 0; z < items.size(); z++)
				oFile << items[z] << endl;
			oFile.close();
		} else if (argList[i] == "<shorthand>") {
			displayShorthandHelpMenu(argListSize, argList);
			break;
		}
	}
	
	std::cout << std::endl << std::endl;
	Pause(true);
}

void displayShorthandHelpMenu(int argc,
		std::vector<std::string> argv) {
	// Determined lines should be auto-numbered
	if (fileExists(argv[0].c_str())) {
		const int WIDTH = 6;
		std::ifstream *file = readFile(argv[0].c_str());
		int inc = 0;
		while (!file->eof()) {
			// Get the data and populate the line string
			std::string lineItem;
			getline(*file, lineItem);
			
			if (lineItem.find("[DEGREE;]") != std::string::npos) {
				// Msys/MinGW and Command Prompt use different
				// character sets, or the degree symbol is different,
				// so we must pass in <mingw> from the terminal when
				// we wish to use that
				bool isMsys = false;
				for (int i = 0; i < argc; i++) {
					if (argv[i] == "<mingw>") {
						isMsys = true;
						break;
					}
				}
				if (isMsys)
					lineItem.replace(lineItem.find("[DEGREE;]"), 9, "\xB0");
				else
					lineItem.replace(lineItem.find("[DEGREE;]"), 9, "\370");
			}
			
			// Output the data to the screen
			std::cout << std::setw(WIDTH) << ++inc << ".  " <<
					lineItem << std::endl;
		}
		closeReadFile(file);
	}
}

bool isMatchingString(std::string haystack,
		std::string needle) {
	std::string::size_type found;
	bool isMatch = false;
	std::string tmpHaystack;
	std::string tmpNeedle;
	for (int i = 0; i < haystack.size(); i++)
		tmpHaystack += toupper(haystack[i]);

	for (int q = 0; q < needle.size(); q++)
		tmpNeedle += toupper(needle[q]);
	
	found = tmpHaystack.find(tmpNeedle);
	isMatch = (found != std::string::npos);
	
	// Return the value
	return isMatch;
}

std::vector<std::string> RemoveDuplicateEntries(
		std::vector<std::string> entries) {
	// Initialize a blank vector to populate
	std::vector<std::string> singleEntries;
	
	for (int inEntries = 0; inEntries < entries.size(); inEntries++) {
		// Get the current entry
		std::string entry = entries[inEntries];
		if (entry == "") // Strip null entries;
			continue;
		bool isMatch = false;
		for (int remEntry = 0; remEntry < singleEntries.size();
				remEntry++) {
			// If the value we have is currently in the singleEntries
			// list, set isMatch to true and break
			if (entry == singleEntries[remEntry]) {
				isMatch = true;
				break;
			}
		}
		if (!isMatch)
			singleEntries.push_back(entry);
	}
	
	// Return the value
	return singleEntries;
}


/* *********************************************************************
// Main function
int main(int argCount, char* fileName[]) {
	if (fileExists(fileName[0])) {
		const int WIDTH = 6;
		std::ifstream *file = readFile(fileName[0]);
		int inc = 0;
		std::vector<std::string> myEntries;
		while (!file->eof()) {
			// Increment the line number
			inc++;
			// Get the data and populate the line string
			std::string lineItem;
			getline(*file, lineItem);
			myEntries.push_back(lineItem);
			// Output the data to the screen
		}
		std::vector<std::string> singles =
				RemoveDuplicateEntries(myEntries);
		for (int i = 0; i < singles.size(); i++)
			std::cout << singles[i] << std::endl;
			
	}
	std::cout << std::endl << std::endl;
	Pause(true);
}
********************************************************************* */






/* *************************************
G:\t\${backups}\_WORK_THURSDAY2011DEC15T0814\_work444\laptop-hpp\D0212032013T1630ESTzUTC-5
G:\t\${backups}\_WORK_THURSDAY2011DEC15T0814\_work444\laptop-hpp\D0212032013T1630ESTzUTC-5\${PRIVATE_VIDEOS}
G:\t\${backups}\_WORK_THURSDAY2011DEC15T0814\_work444\laptop-hpp\D0508022013T2303ESTzUTC-5\SDCard-Phone
************************************* */
			//if (lineItem.find("[DEGREE;]") != std::string::npos)
			//	lineItem.replace(lineItem.find("[DEGREE;]"), 9, "\370");






/*
void displayShorthandHelpMenu(int argc,
		std::vector<std::string> argv) {
	////////////////////////////////////////////////////////
	// UPDATE: Verify, but surely the options should always
	// be numbered...findings? Research!
	// TODO: Determine if number switch required for
	// shorthand help menu items, or if that should be
	// the default action
	////////////////////////////////////////////////////////
	// Check if the options should be numbered
	bool numberOptions = false;
	for (int i = 0; i < argListSize; i++) {
		if ((std::string)argList[i] == "/N") {
			numberOptions = true;
			break;
		}
	}
	
	if (fileExists(argv[0].c_str())) {
		const int WIDTH = 6;
		std::ifstream *file = readFile(argv[0].c_str());
		int inc = 0;
		while (!file->eof()) {
			// Get the data and populate the line string
			std::string lineItem;
			getline(*file, lineItem);
			if (lineItem.find("[DEGREE;]") != std::string::npos)
				lineItem.replace(lineItem.find("[DEGREE;]"), 9, "\xB0");
			
			// Output the data to the screen
			if (numberOptions)
				std::cout << std::setw(WIDTH) << ++inc << ".  " <<
						lineItem << std::endl;
			else
				std::cout << ".  " << lineItem << std::endl;
		}
		closeReadFile(file);
	}
}
*/