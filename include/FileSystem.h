#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <string>
#include <dirent.h>

std::vector<std::string> getDirectoryListing(std::string SPECIFIED_DIRECTORY);

void copyFilesInDirectory(std::string FROM_DIRECTORY,
		std::string TO_DIRECTORY,
		std::vector<std::string> FILES);

#endif