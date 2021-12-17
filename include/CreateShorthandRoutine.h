#ifndef CREATESHORTHANDROUTINE_H
#define CREATESHORTHANDROUTINE_H

// Includes
#include <string>
#include <vector>

struct ShorthandRoutineStruct {
	std::string textFileName;
	std::string csvFileName;
	std::string fullFilePath;
	std::string fullCSVFilePath;
	std::string weeklyFolderName;
	std::vector<std::string> shorthandData;
};

bool CreateShorthandRoutine();

std::vector<std::string> createShorthandFile(
		struct PUIVisibleDTS routineDate,
		std::string routineTime,
		std::vector<std::string> shorthandData);

void writeTXTData(std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData);

void writeCSVData(std::string fullCSVFilePath,
		std::string weeklyFolderName,
		std::string routineDateTime,
		std::vector<std::string> shorthandData);

void writeSimpleView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS);

void writeLongView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS);

void writeListColumnView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS);

void _specTimeOutput(std::ofstream &shorthandFile,
		std::string routineName,
		std::vector<std::string> dataToWrite,
		int timePos);

bool isEnd(std::string endCommand);

std::vector<std::string> getShorthandData();

std::string removeSpaces(std::string input);

bool hasOnlyValidCharsIntXxSpace(std::string &checkMe);

bool isValidColonHMS(std::string &checkMe);

std::vector<int> getValidSetList(std::string inputSet);

int validateRoutineSetCount(std::string routine);

void dumpShorthandRoutineData(std::string filePath,
		struct ShorthandRoutineStruct shorthandRoutineData);

#endif