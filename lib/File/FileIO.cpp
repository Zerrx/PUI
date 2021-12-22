// Function Prototypes
// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <sys/stat.h>
// Regular includes
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\include\conf.h"
#include "..\..\include\ConfigurationFile.h"
#include "..\..\include\DateTime.h"
#include "..\..\include\FileIO.h"
#include "..\..\include\ShellControls.h"
#include "..\..\include\WindowsConsole.h"

using namespace std;

using namespace PUI;

bool directoryExists(const char *directoryName) {
	struct _stat buffer;
	// Initialize the return boolean to false
	bool directoryExists = false;
	memset((void*)&buffer, 0, sizeof(buffer));
	// Get the check result
	int checkValue = _stat(directoryName, &buffer);
	if ((checkValue == 1) || (!((buffer.st_mode) & (_S_IFDIR))))
		// The directory does not exist
		directoryExists = false;
	else
		// The directory exists
		directoryExists = true;
	
	// Return the value
	return directoryExists;
}

bool fileExists(const char *fileName) {
	// Create the stream for the file
	std::ifstream ifile(fileName);
	
	// Returning the value will set the
	// bool to true or false
	
	// Return the value
	return ifile.good();
}

bool overwriteFile(const char *fileName) {
	// Initialize the boolean to overwrite or not
	// Initialized to true if the file does not exist
	bool isOverwrite = true;
	
	// Initialize the user input variable to set overwrite command/boolean
	std::string overwrite;
	
	// If the file exists, query the user to overwrite the file or not
	if (fileExists(fileName)) {
		while (true) {
			std::cout << "\n\nFile exists! Okay to overwrite? (y/Y n/N)\n> ";
			getline(std::cin, overwrite);
			if ((overwrite == "y") || (overwrite == "Y")) {
				break;
			} else if ((overwrite == "n") || (overwrite == "N")) {
				isOverwrite = false;
				break;
			} else {
				std::cout << "Invalid command!";
			}
		}
	}
	
	// Return the value
	return isOverwrite;
}

std::ifstream *readFile(const char *fileName) {
	std::ifstream *fileStream = 0;
	// Open the file specified
	fileStream = new std::ifstream(fileName);
	// Since checks to see if the file name entered already
	// exists have already occurred, open the file
	// Return the value
	return fileStream;
}

void closeReadFile(std::ifstream *fileStream) {
	if (fileStream->good())
		fileStream->close();
	// Try to delete the stream if it exists
	try {
		delete fileStream;
	} catch (...) {/* ... is catch-all. Do nothing? */}
}

//void createDirectoryStructure(const char *directoryPath) {
//	if ((CreateDirectory(directoryPath, NULL)) ||
//			(ERROR_ALREADY_EXISTS == GetLastError()) {
//	}
//}

bool isWhitespace(char content) {
	// Initialize boolean
	bool isCharWhitespace = false;
	
	// Set the whitespace array to check against
	char WhitespaceArray[4] = {'\n', '\t', '\0', ' '};
	
	// Set the counter
	int counter = 0;
	
	// Do the check
	while (counter < 4) {
		if (content == WhitespaceArray[counter]) {
			isCharWhitespace = true;
			break;
		}
		counter++;
	}
	
	// Return the value
	return isCharWhitespace;
}

bool isBreakableHere(char content[], int sizeOfContent) {
	// Initialize the isContentChar boolean
	// Assume false because it may not be a character
	bool itIsBreakableHere = false;
	
	char SpecialBreakArray[5] = {'-','/','\\','|',':'};
	char SpecialBreakArrayOnLongLine[6] = {'-','/','\\','|',':',';'};
	
	// Initialize CharacterArray counter
	int counter = 0;
	
	// Check for special breaks, if not breakable, no point
	// in checking for whitespace, then
	if (sizeOfContent == 2) {
		while (counter < 5) {
			bool firstIsMatch = (content[0] == SpecialBreakArray[counter]);
			char secondChar[1];
			secondChar[0] = content[1];
			bool secondIsMatch = isBreakableHere(secondChar,1);
			// If first and second character are matches, then
			// this special case requires backtracking
			if (firstIsMatch && secondIsMatch)
				break;// No need to set false as that is already set
			else if (firstIsMatch) {
				itIsBreakableHere = true;
				break;
			}
			counter++;
		}
	} else if (sizeOfContent == 1) {
		while (counter < 6) {
			itIsBreakableHere = (content[0] ==
					SpecialBreakArrayOnLongLine[counter]);
			// If first character is match, break in line reached
			// and no point in checking further
			if (itIsBreakableHere)
				break;
			counter++;
		}
	}
	
	// If we cannot break on special chars, then check to
	// see if the next character is a whitespace: if it is
	// we can break, else-wise we cannot
	if (!itIsBreakableHere) {
		if (sizeOfContent == 1) {
			if (isWhitespace(content[0])) {
				itIsBreakableHere = true;
			}
		} else if (isWhitespace(content[1])) {
			itIsBreakableHere = true;
		}
	}
	
	// Return the value
	return itIsBreakableHere;
}

std::string getBinary(const unsigned char val) {
	std::string returnVal;
	for(int i = 7; i >= 0; i--) {
		if(val & (1 << i))
			returnVal += "1";
		else
			returnVal += "0";
	}
	
	// Return the value
	return returnVal;
}

std::vector<std::vector<std::string> > decodeBinaryFile(const char *fileName) {
	std::vector<std::vector<std::string> > txt;
	std::vector<std::string> fn;
	std::ifstream in;
	in.open(fileName, std::ios::binary);
	std::string tmp;
	int im = 0;
	if (in.is_open()) {
		while (true) {
			char character = in.get();
			if (in.tellg() == EOF)
				break;
			if ((character == '\r') || (character == '\n')) {
				if (!fn.size() && tmp.size()) {
					fn.push_back(tmp);
					tmp.clear();
					txt.push_back(fn);
					fn.clear();
				} else if (fn.size()) {
					txt.push_back(fn);
					fn.clear();
				}
				continue;
			}
			std::string s = getBinary(((int)((unsigned char)character)));
			unsigned int charNum = 0;
			int exponent = -1;
			for (int j = 7; j >= 0; j--) {
				// Increment the exponent as we iterate through
				// the byte string to raise the power according
				// to binary rules for adding
				exponent++;
				// Convert ASCII num to int num
				int num = (s[j] - '0');
				charNum += ((num == 1) ? std::pow(2, exponent) : 0);
			}
			std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
			secureReadVal = secureReadVal.substr(2,secureReadVal.size());
			int srv;
			std::istringstream streamReadVal(secureReadVal);
			streamReadVal >> srv;
			charNum -= srv;
			char c = (unsigned char)(unsigned int)charNum;
			if ((c == '\r') || (c == '\n')) {
				// Windows uses \r\n to create new lines...therefore,
				// in order to out the data correctly, we must not
				// populate the return "txt" variable with null data
				// or new line data
				if (tmp.size()) {
					fn.push_back(tmp);
					tmp.clear();
				}
			} else
				tmp += c;
		}
		if (fn.size())
			txt.push_back(fn);
	}
	
	// Close the file
	in.close();
	
	// Return the value
	return txt;
}

std::vector<std::string> getEncodedFilesList(std::string inFile) {
	std::vector<std::string> files;
	
	std::ifstream in;
	in.open(inFile.c_str(), ios::in | ios::binary);
	
	if (!in.is_open()) {
		in.close();
		
		// Return the value
		return files; // Write failed, we cannot proceed
	}
	
	std::string filesLine = "";
	std::string tmpFilesLine;
	getline(in, tmpFilesLine);
	int filesLineBeginPos = 0;
	int sizeOfTmpFilesLine = tmpFilesLine.size();
	while (filesLineBeginPos < sizeOfTmpFilesLine) {
		std::string ibsonComma = tmpFilesLine.substr(filesLineBeginPos, 3);
		filesLineBeginPos += 4;
		int cNum;
		std::istringstream streamNum(ibsonComma);
		streamNum >> cNum;
		std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
		secureReadVal = secureReadVal.substr(2,secureReadVal.size());
		int srv;
		std::istringstream streamReadVal(secureReadVal);
		streamReadVal >> srv;
		char c = (char)(cNum - srv);
		if (c != ';') {
			filesLine += c;
		} else {
			files.push_back(filesLine);
			filesLine.clear();
		}
	}
	
	in.close();
	
	// Return the value
	return files;
}

std::vector<std::string> getEncodedFilesListFromMemory(std::vector<std::vector<std::string> > binaryFileVector) {
	std::vector<std::string> files;
	
	std::string filesLine = "";
	std::string tmpFilesLine = binaryFileVector[0][0];
	int filesLineBeginPos = 0;
	int sizeOfTmpFilesLine = tmpFilesLine.size();
	while (filesLineBeginPos < sizeOfTmpFilesLine) {
		std::string ibsonComma = tmpFilesLine.substr(filesLineBeginPos, 3);
		filesLineBeginPos += 4;
		int cNum;
		std::istringstream streamNum(ibsonComma);
		streamNum >> cNum;
		std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
		secureReadVal = secureReadVal.substr(2,secureReadVal.size());
		int srv;
		std::istringstream streamReadVal(secureReadVal);
		streamReadVal >> srv;
		char c = (char)(cNum - srv);
		if (c != ';') {
			filesLine += c;
		} else {
			files.push_back(filesLine);
			filesLine.clear();
		}
	}
	
	// Return the value
	return files;
}

bool writeDecodedBinaryFile(std::string inFile, std::string outFile, std::string fileName) {
	if (!fileExists(inFile.c_str())) {
		// Return the value
		return false;
	}

	std::vector<std::string> files = getEncodedFilesList(inFile);
	
	if (!files.size()) {
		// Return the value
		return false;
	}
	
	int filePosition = 0;
	
	int numberOfFiles = files.size();
	for (int i = 0; i < numberOfFiles; i++) {
		if (files[i] == fileName) {
			filePosition = (i + 1);
			break;
		}
	}
	
	if (!filePosition) {
		// No matching file found
		
		// Return the value
		return false;
	}

	ifstream in;
	in.open(inFile.c_str(), ios::in | ios::binary);
	
	if (!in.is_open()) {
		in.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	ofstream out;
	out.open(outFile.c_str(), ios::out | ofstream::binary);
	
	if (!out.is_open()) {
		out.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	std::string line;
	int currentLine = 0;
	
	while (currentLine <= filePosition) {
		getline(in, line);
		// In case bad value given, check for EOF
		if ((in.tellg() == EOF) && (currentLine != filePosition)) {
			in.close();
			out.close();
			return false;
		} else if (currentLine < filePosition) {
			line.clear();
			currentLine++;
			continue;
		}
		std::string ibsonComma;
		int begPos = 0;
		while (begPos < line.size()) {
			ibsonComma = line.substr(begPos, 3);
			begPos += 4;
			int cNum;
			std::istringstream streamNum(ibsonComma);
			streamNum >> cNum;
			std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
			secureReadVal = secureReadVal.substr(2,secureReadVal.size());
			int srv;
			std::istringstream streamReadVal(secureReadVal);
			streamReadVal >> srv;
			out << (char)(cNum - srv);
		}
		
		// Get out of the loop
		break;
	}
	
	in.close();
	out.close();
	
	// Return the value
	return true;
}

bool writeDecodedBinaryFileFromMemory(std::vector<std::string> files,
		std::vector<std::vector<std::string> > binaryFileVector,
		std::string outFile,
		std::string fileName) {
	
	if (!files.size()) {
		// Return the value
		return false;
	}
	
	int filePosition = 0;
	
	int numberOfFiles = files.size();
	for (int i = 0; i < numberOfFiles; i++) {
		if (files[i] == fileName) {
			filePosition = (i + 1);
			break;
		}
	}
	
	if (!filePosition) {
		// No matching file found
		
		// Return the value
		return false;
	}

	ofstream out;
	out.open(outFile.c_str(), ios::out | ofstream::binary);
	
	if (!out.is_open()) {
		out.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	std::string line;
	int currentLine = 0;
	
	while (currentLine <= filePosition) {
		line = binaryFileVector[0][currentLine];
		// In case bad value given, check for EOF
		if (currentLine < filePosition) {
			line.clear();
			currentLine++;
			continue;
		}
		std::string ibsonComma;
		int begPos = 0;
		while (begPos < line.size()) {
			ibsonComma = line.substr(begPos, 3);
			begPos += 4;
			int cNum;
			std::istringstream streamNum(ibsonComma);
			streamNum >> cNum;
			std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
			secureReadVal = secureReadVal.substr(2,secureReadVal.size());
			int srv;
			std::istringstream streamReadVal(secureReadVal);
			streamReadVal >> srv;
			out << (char)(cNum - srv);
		}
		
		// Get out of the loop
		break;
	}
	
	out.close();
	
	// Return the value
	return true;
}

void CreateZipFolder(std::string commandsAndSwitches) {
	std::string executable = "C:/Program Files/7-Zip/7z.exe";
	long lengtha = 0;
	TCHAR* bufferzone = NULL;
	lengtha = GetShortPathName(executable.c_str(), NULL, 0);
	if (lengtha == 0)
		std::cout << "\n\n\n\nFATAL ERROR!\n\n\n\n\n";
	bufferzone = new TCHAR[lengtha];
	lengtha = GetShortPathName(executable.c_str(), bufferzone, lengtha);
	if (lengtha == 0)
		std::cout << "\n\n\n\nFATAL ERROR!\n\n\n\n\n";
	std::string makeArguments = ((std::string)bufferzone + " " + commandsAndSwitches);
	_createZipFolder(executable, bufferzone, makeArguments.c_str());
}

void _createZipFolder(std::string executable,
		TCHAR* bufferzone,
		const char *arguments) {
	// This function is a workaround for the
	// const char * requirements of CreateProcess
	// Additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	// Set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	// Start the program up
	CreateProcess(executable.c_str(), // the path
			(char *)arguments, // Command line
			NULL, // Process handle not inheritable
			NULL, // Thread handle not inheritable
			false, // Set handle inheritance to false
			CREATE_NO_WINDOW, // Creation flags
			NULL, // Use parent's environment block
			NULL, // Use parent's starting directory
			&si, // Pointer to STARTUPINFO structure
			&pi); // Pointer to PROCESS_INFORMATION structure
	// Close process handle
	CloseHandle(pi.hProcess);
	// Close thread handle
	CloseHandle(pi.hThread);
}

// TODO: Add config entry and get config entry values