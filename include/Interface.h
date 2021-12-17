#ifndef INTERFACE_H
#define INTERFACE_H

// Includes
#include <string>

bool getInterface();

void setNewCommandLine(void);

void getIntroduction(std::string appName);

bool isMatchingString(std::string haystack,
		std::string needle);

bool isClose(std::string command);

int inputHandler();

bool isValidCommand(std::string command);

int executeCommand(std::string command);

bool isHelp(std::string command);

void executeHelp();

//int getIntroductionContents(std::ifstream *introductionsStream,
int getIntroductionContents(std::string fileName,
		const int DISPLAY_SIZE,
		std::string INTRO_FOR);

void help(const char *operateFilePath);

void loadApplicationData(std::string inFile, int encOpt);

std::vector<std::vector<std::string> > decodeBinaryFile(const char *fileName, int isEncoded);

std::string getBinary(const unsigned char val);

std::vector<std::string> getApplicationData(std::string sKey);

#endif