// TODO: Find and download songs similar to, and in the same genre as, "The Rocky Road To Dublin" by The Dubliners

// System and Special includes
#include <Shlobj.h> // For SHGetFolderPath
#include <cstdio>
#include <cstdlib>
#include <tchar.h>
#include <windows.h>
// Regular includes
#include <cctype>
#include <cmath>
#include <ctime>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <direct.h>
#include <fstream>
// Application includes
#include "..\..\include\Bodybuilding.h"
#include "..\..\include\conf.h"
#include "..\..\include\ConfigurationFile.h"
#include "..\..\include\Construction.h"
#include "..\..\include\FileIO.h"
#include "..\..\include\Interface.h"
#include "..\..\include\ShellControls.h"
#include "..\..\include\WindowsConsole.h"
#include "..\..\include\Writing.h"

using namespace std;

using namespace PUI;

bool getInterface() {
	// Initialize the quit variable
	bool isNotInterfaceQuit = true;
	
	// Clear the screen
	ClearScreen();


	// Get the user's home directory
	TCHAR szPath[MAX_PATH];
	std::string userHomeDirectory;
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, szPath))) {
		userHomeDirectory = szPath;
		userHomeDirectory = userHomeDirectory.substr(0, (userHomeDirectory.find_last_of('\\') + 1));
		if (config.getValue("Application.InstallDirectory") == "") {
			config.setValue("Application.InstallDirectory", PUI::applicationPath.c_str());
			config.save();
		}
		if (config.hasValue("Application.PrimaryDirectory")) {
			if (config.getValue("Application.PrimaryDirectory") == "") {
				config.setValue("Application.PrimaryDirectory", userHomeDirectory.c_str());
				std::string sportsDir = (userHomeDirectory + "Documents\\Sports\\");
				std::string bbd = (sportsDir + "Bodybuilding\\");
				config.setValue("Module.Sports.Directory", sportsDir.c_str());
				config.setValue("Module.Sports.Bodybuilding.Directory", bbd.c_str());
				config.save();
			}
		}
		ofstream dog;
		dog.open((userHomeDirectory + "Documents\\Tess.txt").c_str());
		dog << "My dog's name is Tess, and I love her very much! She is a blessing unto me, because the Lord loves me and is always listening and always answers my prayers...even though I do not always like what I hear, or do not see results \"soon enough\".";
		dog.close();
	}


	// Get the introduction
	getIntroduction("main");
	
	// Check for quit!
	if (inputHandler() == 0) {
		std::cout << "\n\n\n\n\nClosing the application now...\n\n\n\n\n";
		isNotInterfaceQuit = false;
	}
	
	// Return the value
	return isNotInterfaceQuit;
}

void setNewCommandLine(void) {
	std::cout << "\n\n> ";
}

void getIntroduction(std::string appName) {
	std::string appIntroFile = config.getValue("DATAPROCF.Application.Introduction");
	if (!PUI::appData.size())
		loadApplicationData(appIntroFile, 2);
	// Initialize the display size
	std::string displaySizeValue = PUI::config.getValue("Application.DisplaySize");
	int DISPLAY_SIZE = atoi(displaySizeValue.c_str());
	// Initialize the file handler
	std::string fileName = "..\\auxfiles\\INTRODUCTIONS";
	std::string tbBorder = "";
	for (int i = 0; i < DISPLAY_SIZE; i++)
		tbBorder += "*";
	std::cout << std::endl << std::endl << tbBorder;
	std::cout << std::endl;
	int fileContents = getIntroductionContents(fileName, DISPLAY_SIZE,
			("[INTERFACE: " + appName + "]"));
	std::cout << tbBorder;
	std::cout << std::endl;
}

bool isMatchingString(std::string haystack,
		std::string needle) {
	std::string::size_type found;
	bool isMatch = false;
	std::string tmpHaystack;
	std::string tmpNeedle;
	
	int sizeOfHaystack = haystack.size();
	for (int i = 0; i < sizeOfHaystack; i++)
		tmpHaystack += toupper(haystack[i]);
	
	int sizeOfNeedle = needle.size();
	for (int q = 0; q < sizeOfNeedle; q++)
		tmpNeedle += toupper(needle[q]);
	
	found = tmpHaystack.find(tmpNeedle);
	isMatch = (found != std::string::npos);
	
	// Return the value
	return isMatch;
}

bool isClose(std::string command) {
	// Initialize the close command boolean
	bool isCloseCommanded = false;
	
	// Check for the close command(s)
	if ((command == "quit") ||
		(command == "close") ||
		(command == "cancel") ||
		(command == "exit"))
			isCloseCommanded = true;
	else
		isCloseCommanded = false;
		
	// Return the value
	return isCloseCommanded;
}

int inputHandler() {
	std::string command;
	while (true) {
		setNewCommandLine();
		getline(std::cin, command);
		if (isClose(command))
			return 0;
		else if (isValidCommand(command))
			return executeCommand(command);
		else if (isHelp(command))
			executeHelp();
		else
			std::cout << "\nInvalid command or option is not available, please try again!";
	}
}

bool isValidCommand(std::string command) {
	// Initialize whether the command is valid, assume user
	// input is error, proceed with validation.
	bool validCommand = false;
	
	// Validate command
	if ((command == "a") ||
		(command == "i") ||
		(command == "j") ||
		(command == "k"))
		validCommand = true;
	
	// Return the value
	return validCommand;
}

int executeCommand(std::string command) {
	// Initialize the future, noError integer that will
	// return the error number for help in determining
	// the cause of the problem.
	int noError = 1;
	if (command == "a") {
		while (true) {
			if (!Bodybuilding())
				break;
			// Repeat until quit
		}
		noError = 1;
	} else if (command == "i") {
		while (true) {
			if (!Writing())
				break;
			// Repeat until quit
		}
		noError = 1;
	} else if (command == "j") {
		while (true) {
			if (!Construction())
				break;
			// Repeat until quit
		}
		noError = 1;
	} else if (command == "k") {
		std::string helpPath = "C:\\Env\\Dev\\PUI\\app\\PUI\\PUI.cpp";
		help(helpPath.c_str());
		noError = 1;
	}
	
	// Return the value
	return noError;
}

bool isHelp(std::string command) {
	// Initialize the requestHelp boolean
	bool requestHelp = false;
	if (command == "help")
		requestHelp = true;
	
	// Return the value
	return requestHelp;
}

void executeHelp() {
	std::string helpLink = "C:\\Env\\Dev\\$_Programming\\C++\\$_help\\reference\\en\\index.html";
	ShellExecute(NULL, NULL, helpLink.c_str(), NULL, NULL, SW_SHOW);
}

int getIntroductionContents(std::string fileName,
		const int DISPLAY_SIZE,
		std::string INTRO_FOR) {
	// Return 1: No error; Return -1: An error has occurred
	int getFileContents = 1;
	// When we have found the intro we need to begin outputting
	// data until we have found the end of the data...
	// we have found the end of the data, but only after having
	// found the introduction we want, we need to break
	bool foundIntro = false;
	std::vector<std::string> introData = getApplicationData(fileName);
	int introSize = introData.size();
	for (int line = 0; line < introSize; line++) {
		// Do the checks for the lines
		// read a value
		std::string fileLine = introData[line];
		if ((!foundIntro) && (!isMatchingString(fileLine,
				INTRO_FOR))) {
			continue;
		} else if (isMatchingString(fileLine,
				INTRO_FOR)) {
			foundIntro = true;
			continue;
		} else if (isMatchingString(fileLine, "[END:INTERFACE;]"))
				break;
		if (isMatchingString(fileLine, "[PAR_SEP]"))
			fileLine.replace(0, 9, "\n");
		else if (isMatchingString(fileLine, "[PAR]"))
			fileLine.replace(0, 5, "");
		int strLen = fileLine.length();
		if (strLen > DISPLAY_SIZE) {
			int counter = 0;
			int CHAR_SIZE = strLen;
			const char *dispChars = new char[CHAR_SIZE+1];
			dispChars = fileLine.c_str();
			while (counter <= strLen) {
				std::string tmp_str = "";
				const char *outputChars = new char[CHAR_SIZE];
				// put output chars together;
				int newPoint = counter;
				int outputPoint = 0;
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				// For some reason, without inputting this = with
				// the less than, funny characters, weird things,
				// and other undesirable behavior occurs
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				while (newPoint <= strLen) {
					tmp_str += dispChars[newPoint];
					newPoint++;
					outputPoint++;
				}
				outputChars = tmp_str.c_str();
				// If counter == strLen, break: we have output
				// all data
				if (counter == strLen)
					break;
				char CheckNextSequence[2];
				// Sequence point: this will be the length of the
				// characters we are working with to make life easier
				int sequencePoint = 0;
				while ((sequencePoint < outputPoint) ||
					   (sequencePoint < DISPLAY_SIZE)) {
					if (((sequencePoint + 1) == outputPoint) || ((sequencePoint + 2) > outputPoint)){
						CheckNextSequence[0] = outputChars[sequencePoint+1];
						CheckNextSequence[1] = ' ';
						sequencePoint++;
						break;
					} else if ((sequencePoint + 1) == DISPLAY_SIZE) {
						CheckNextSequence[0] = outputChars[sequencePoint];
						CheckNextSequence[1] = outputChars[sequencePoint+1];
						sequencePoint++;
						break;
					}
					sequencePoint++;
				}
				if (!isBreakableHere(CheckNextSequence, 2)) {
					// To check if whitespace
					int countBack = (sequencePoint-1);
					bool isBroke = true;
					while (!isWhitespace(outputChars[countBack])) {
						countBack--;
						// Special Case check: long line of characters,
						// probably used for headers or sectioning
						if (countBack == -1) {
							// ADD SequencePoint to reset countBack to
							// last-known sequencePoint for breaking
							countBack = sequencePoint;
							isBroke = false;
							break;
						}
					}
					if (isBroke)
						countBack++;
					int breakablePoint = 0;
					// Do the output
					while (breakablePoint < countBack) {
						// Replace tab characters for visual aid
						// TODO: add and check for position indicators
						// that will tell the saveFile function where
						// tabs were replaced.
						if (outputChars[breakablePoint] == '\t')
							std::cout << "    ";
						else
							std::cout << outputChars[breakablePoint];
						breakablePoint++;
					}
					if (isBroke)
						std::cout << std::endl;
					counter += countBack;
				} else {
					int breakablePoint = 0;
					while (breakablePoint < sequencePoint) {
						// Replace tab characters for visual aid
						// TODO: add and check for position indicators
						// that will tell the saveFile function where
						// tabs were replaced.
						if (outputChars[breakablePoint] == '\t')
							std::cout << "    ";
						else
							std::cout << outputChars[breakablePoint];
						breakablePoint++;
					}
					std::cout << std::endl;
					counter += sequencePoint;
				}
			}
		} else {
			// Replace tab characters for visual aid
			// TODO: add and check for position indicators
			// that will tell the saveFile function where
			// tabs were replaced.
			std::string::size_type foundPos = fileLine.find('\t');
			while (foundPos != std::string::npos) {
				fileLine.replace(foundPos, 1, "    ");
				foundPos = fileLine.find('\t');
			}
			fileLine.resize(fileLine.size());
			std::cout << fileLine << std::endl;
		}
	}
	
	// Return the value
	return getFileContents;
}

void help(const char *operateFilePath) {
	std::string executable = "C:\\Env\\Dev\\PUI\\app\\PUI\\help.exe";
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// start the program up
	CreateProcess(executable.c_str(), // the path
			(char *)operateFilePath, // Command line
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			false, // Set handle inheritance to false
			CREATE_NEW_CONSOLE, // Creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory
			&si, // Pointer to STARTUPINFO structure
			&pi); // Pointer to PROCESS_INFORMATION structure
	// Close process handle
	CloseHandle(pi.hProcess);
	// Close thread handle
	CloseHandle(pi.hThread);
}

void loadApplicationData(std::string inFile, int encOpt = 2) {
	// Decode the file for the selected value
	std::vector<std::vector<std::string> > decoded = decodeBinaryFile(inFile.c_str());
	std::vector<std::string> files;
	int pos = 0;
	std::string fileLine = decoded[0][0];
	std::string::size_type semi = fileLine.find(';');
	while (semi != std::string::npos) {
		std::string file = fileLine.substr(pos, semi - pos);
		files.push_back(file);
		pos = (semi + 1);
		semi = fileLine.find(';', pos);
	}
	int numberOfFiles = files.size();
	
	for (int i = 0; i < numberOfFiles; i++) {
		PUI::appData.insert(make_pair(files[i], decoded[i+1]));
	}
}

std::vector<std::string> getApplicationData(std::string sKey) {
	if (PUI::appData.find(sKey.c_str()) != PUI::appData.end()) {
		std::map<std::string, std::vector<std::string> >::iterator iter;
		iter = PUI::appData.find(sKey.c_str());
		return (*iter).second;
	} else {
		std::vector<std::string> n;
		n.push_back("SEARCH_STRING_NOT_FOUND!!!");
		return n;
	}
}

/* ******************************************
palladium ~ noun    very rare

 1. a silver-white metallic element of the platinum group that resembles platinum; occurs in some copper and nickel ores; does not tarnish at ordinary temperatures and is used (alloyed with gold) in jewelry
****************************************** */



/* *********************************************************************
REMOVALS:
--------------
#include "..\DateTime\DateTime.h"
#include "..\File\FileIO.h"













	//std::ifstream *introductionsStream = readFile(fileName.c_str());
	//int fileContents = getIntroductionContents(introductionsStream,

	
	//# TODO: UPDATE CreateNewWeeklyDirectory AND CreateShorthandRoutine FILES, AND THE getIntroduction() function TO MATCH NEW CONFIG NAME CHANGES




int help(const char *operateFilePath) {
	// Handles
	HANDLE g_hChildStd_IN_Rd = NULL;
	HANDLE g_hChildStd_IN_Wr = NULL;
	HANDLE g_hChildStd_OUT_Rd = NULL;
	HANDLE g_hChildStd_OUT_Wr = NULL;
	HANDLE g_hInputFile = NULL;
	
	std::string executable = "C:\\Env\\Dev\\PUI\\app\\PUI\\help.exe";
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	SECURITY_ATTRIBUTES saAttr;
	// Set the bInheritHandle flag so pipe handles are inherited
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = true;
	saAttr.lpSecurityDescriptor = NULL;
	// Create a pipe for the child process's STDOUT
	if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0))
		return -1; // Child's stdout pipe creation failed
	// Ensure the read handle to the pipe for STDOUT is not inherited
	if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
		return -2; // Setting handle information failed
	// Create a pipe for the child process's STDIN
	if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
		return -3; // Child's stdin pipe creation failed
	// Ensure the write handle to the pipe for STDIN is not inherited
	if (!SetHandleInformation(g_hChildStd_IN_Rd, HANDLE_FLAG_INHERIT, 0))
		return -4; // Setting handle information failed
	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// start the program up
	CreateProcess(executable.c_str(), // the path
			(char *)operateFilePath, // Command line
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			false, // Set handle inheritance to false
			CREATE_NEW_CONSOLE, // Creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory
			&si, // Pointer to STARTUPINFO structure
			&pi); // Pointer to PROCESS_INFORMATION structure
	// Close process handle
	CloseHandle(pi.hProcess);
	// Close thread handle
	CloseHandle(pi.hThread);
}

#PROJDIRS := categories/construction/ categories/sporting/bodybuilding/ lib/DateTime/ lib/File/ lib/Form lib/Interface/ lib/Math/ lib/ShellControls/ lib/Writing/ include/
#SRCFILES := $(shell find $(PROJDIRS) -type f -name "*.cpp")
#HDRFILES := $(shell find $(PROJDIRS) -type f -name "*.h")
#ALLFILES := $(SRCFILES) $(HDRFILES)
#todolist:
#	-@for fileMM in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$fileMM;



			string s = cf.getValue(argv[i]);
			if (s != "131 Annadell Rd")
				std::cout << "Key " << argv[i] << " = [" << s.c_str() << "]\n";
			else if (s == "131 Annadell Rd") {
				cf.setValue(argv[i], "9144 Simpleton Dr");
				s = cf.getValue(argv[i]);
				hasSetValues = true;
				std::string doSave;
				while (true) {
					std::cout << std::endl << "Change data? (yes / no): ";
					getline(std::cin, doSave);
					if (doSave == "")
						continue;
					else if ((doSave != "yes") && (doSave != "no"))
						continue;
					else if (doSave == "yes") {
						std::cout << "Key " << argv[i] << " = [" << s.c_str() << "]\n";
						break;
					} else if (doSave == "no") {
						hasSetValues = false;
						cf.reset();
						s = cf.getValue(argv[i]);
						std::cout << "Key " << argv[i] << " = [" << s.c_str() << "]\n";
						break;
					}
				}
			}


//class conf : public ConfigurationFile {
//	public:
//		ConfigurationFile cf;
//};

// Initialization
//extern struct app PUI;

//#include <map>
//#include "include\ConfigurationFile.h"

//namespace PUI {
//	ConfigurationFile config;
	//std::vector<std::string> introData;
//	map<std::string, std::vector<std::string> > appData;
//};


********************************************************************* */
	// TODO: Get the DISPLAY_SIZE from the configuration file
		/*
	while (true) {
		in.get(ch);
		if (in.tellg() == EOF)
			break;
		char c[1];
		c[0] = (char)((unsigned int)ch + 314);
		out1.write(c, sizeof(c));
	}
	*/
//		if (in.tellg() == EOF)
//			break;
//		out2 << ((unsigned int)ch - 314);
	//std::string o2 = ("../" + (std::string)argv[1]);
// #LineLength = 70
// # Application.Texts.Directories = 
// # TextDisplayDirectory = 

	/* ******************
		OFSTREAM does not take an std::string as an argument.
		>> Use stringName.c_str() instead
		>> c_str() converts an std::string to a const char*, which an OFSTREAM can use
	****************** */
