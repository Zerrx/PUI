#ifndef CREATENEWWEEKLYDIRECTORY_H
#define CREATENEWWEEKLYDIRECTORY_H

// Includes
#include <string>
#include "DateTime.h"

void makeDirectory(std::string path, int index);

int CreateNewWeeklyDirectory();

void createDirectoryInfoFile(std::string newWeeklyDirectory,
		std::string dirInfoFileName,
		struct DayAndWeekInfo currentDayWeek,
		std::vector<std::string> filesCopied);

std::string GetBodybuildingWeekDateCodeInfo(
		struct PUIVisibleDTS beginDayOfWeek,
		struct PUIVisibleDTS endDayOfWeek);

std::vector<std::string> getDirectoryListing(std::string SPECIFIED_DIRECTORY);

bool writeDecodedBinaryWeeklyRoutinesFilesFromMemory(std::vector<std::string> files,
		std::vector<std::vector<std::string> > binaryFileVector,
		std::string outFile,
		std::string fileName,
		struct DayAndWeekInfo currentDayWeek);

#endif