#ifndef FileIO_H
#define FileIO_H

bool directoryExists(const char *directoryName);

bool fileExists(const char *fileName);

bool overwriteFile(const char *fileName);

std::ifstream *readFile(const char *fileName);

void closeReadFile(std::ifstream *fileStream);

bool isBreakableHere(char content[], int sizeOfContent);

bool isWhitespace(char content);

std::string getBinary(const unsigned char val);

std::vector<std::vector<std::string> > decodeBinaryFile(const char *fileName);

bool writeDecodedBinaryFile(std::string inFile, std::string outFile, std::string fileName);

bool writeDecodedBinaryFileFromMemory(std::vector<std::string> files,
		std::vector<std::vector<std::string> > binaryFileVector,
		std::string outFile,
		std::string fileName);

std::vector<std::string> getEncodedFilesList(std::string inFile);

std::vector<std::string> getEncodedFilesListFromMemory(std::vector<std::vector<std::string> > binaryFileVector);

void CreateZipFolder(std::string commandsAndSwitches);
	void _createZipFolder(std::string executable, TCHAR* bufferzone, const char *arguments);

#endif