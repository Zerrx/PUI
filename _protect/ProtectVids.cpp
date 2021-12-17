// From Thinking in C++, 2nd Edition
// (c) Bruce Eckel 2000

// Regular includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
// Application includes
#include "printBinary.h"
#include "../include/FileIO.h"

using namespace std;

std::string getFileBinary(const char *fileName, int encOpt) {
	// Declare an input file stream
	ifstream in;
	in.open(fileName, std::ios::binary);
	
	std::string bin = "";
	
	if (!in.is_open()) {
		// Close in file
		in.close();
		
		// Return the value
		bin = "false";
	} else {
		while (true) {
			// If we check for the end of file in
			// the loop condition statement, the
			// eof() can return false, then the
			// next get() statement can return
			// the EOF as a char in our loop.
			// This can add a funny character at
			// the end of the file, something we
			// do not want...it is undesirable
			unsigned char c = in.get();
			if (in.tellg() == EOF)
				break;
			unsigned int i = c;
			if ((encOpt == 1) ||
				(encOpt == 0))
				bin += getBinary(c);
				// printBinary(c, out);
			else if (encOpt == 2)
				// TODO: Add config entry and get config entry values
				bin += getBinary(i + 314);
			else if (encOpt == 3)
				// TODO: Add config entry and get config entry values
				bin += getBinary(((int)c + 314));
				// printBinary(((int)c + 314), out);
		}
	}
	
	// Close in file
	in.close();
	
	// Return the value
	return bin;
}

std::string encodeBinaryString(std::string bin, int encodeOption) {
	// The return value
	std::string dataLine = "";
	
	int sizeOfBin = bin.size();
	int i = 0;
	while (i < sizeOfBin) {
		std::string byte = bin.substr(i, 8);
		int charNum = 0;
		int exponent = -1;
		for (int j = 7; j >= 0; j--) {
			// Increment the exponent as we iterate through
			// the byte string to raise the power according
			// to binary rules for adding
			exponent++;
			// Convert ASCII num to int num
			int num = (byte[j] - '0');
			charNum += ((num == 1) ? std::pow(2, exponent) : 0);
		}
		
		// TODO: Add config entry and get config entry values
		// Default 0,1
		char ch = (char)charNum;
		if (encodeOption == 1)
			ch = (char)(int)charNum;
		else if (encodeOption == 2)
			ch = ((unsigned char)((int)charNum));
		
		dataLine += ch;
		i += 8;
	}
	
	// Return the value
	return dataLine;
}

std::vector<std::string> encodeBinaryStringList(std::string bin, int encodeOption) {
	std::vector<std::string> txt;
	int sizeOfBin = bin.size();
	
	std::string dataLine = "";
	int i = 0;
	int next = 80;
	while (i < sizeOfBin) {
		std::string byte = bin.substr(i, 8);
		int charNum = 0;
		int exponent = -1;
		for (int j = 7; j >= 0; j--) {
			// Increment the exponent as we iterate through
			// the byte string to raise the power according
			// to binary rules for adding
			exponent++;
			// Convert ASCII num to int num
			int num = (byte[j] - '0');
			charNum += ((num == 1) ? std::pow(2, exponent) : 0);
		}
		
		// TODO: Add config entry and get config entry values
		// Default 0,1
		char ch = (char)charNum;
		if (encodeOption == 1)
			ch = (char)(int)charNum;
		else if (encodeOption == 2)
			ch = ((unsigned char)((int)charNum));
		
		if (((i + 8) == next) ||
			((i + 8) == sizeOfBin)){
			dataLine += ch;
			txt.push_back(dataLine);
			dataLine.clear();
			next += 80;
		} else
			dataLine += ch;
		i += 8;
	}
	
	// Return the value
	return txt;
}

std::vector<std::string> decodeBinaryFile(const char *fileName, int isEncoded) {
	std::vector<std::string> txt;
	std::ifstream in;
	in.open(fileName, std::ios::binary);
	std::string tmp;
	if (in.is_open()) {
		while (true) {
			char character = in.get();
			if (in.tellg() == EOF)
				break;
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
			// TODO: Add config entry and get config entry values
			charNum -= 314;
			char c = (unsigned char)(unsigned int)charNum;
			if ((character == '\r') || (character == '\n')) {
				// Windows uses \r\n to create new lines...therefore,
				// in order to out the data correctly, we must not
				// populate the return "txt" variable with null data
				// or new line data
				if (tmp.size()) {
					txt.push_back(tmp);
					tmp.clear();
				}
			} else
				tmp += c;
			//if ((c == '\r') || (c == '\n')) {
			//	// Windows uses \r\n to create new lines...therefore,
			//	// in order to out the data correctly, we must not
			//	// populate the return "txt" variable with null data
			//	// or new line data
			//	if (tmp.size()) {
			//		txt.push_back(tmp);
			//		tmp.clear();
			//	}
			//} else
			//	tmp += c;
		}
		int ts = txt.size();
		// Must always ensure the last line is included
		if (!ts || (txt[ts - 1] != tmp))
			txt.push_back(tmp);
	}
	
	// Close the file
	in.close();
	
	// Return the value
	return txt;
}

int main(int argc, char **argv) {
	// Return value 0 is no error; positive otherwise
	int isError = 0;
	// Encoding option: possible values 0-9
	int encOpt = 0;
	// Use a string variable for file names in case we need to
	// do work with the file names
	std::vector<std::string> inFileList;
	std::vector<std::string> nonExistentFiles;
	std::string inFile = "";
	std::string outFile = "";
	bool isListFile = false;
	if (argc == 5) {
		if (((std::string)argv[3] == "\\L") ||
			((std::string)argv[3] == "\\l"))
			isListFile = true;
	}
	if (argc == 4) {
		std::istringstream(argv[3]) >> encOpt;
		if ((encOpt > 9) || (encOpt < 0))
			// Return the value
			isError = 1;
		if (!fileExists(argv[1]))
			isError = 2;
		if (!fileExists(argv[2]))
			isError = 2;
		inFile = (std::string)argv[1];
		outFile = (std::string)argv[2];
	} else if (isListFile) {
		// Only one input...must be a file and the file must
		// exist, and the file must have valid URLs and they
		// can exist by at least one: if none of the items
		// exist return error state 3; if one of the items
		// exist, continue. If the input is not a file...or
		// the file does not exist...return error state 2.
		if (!fileExists(argv[1]))
			isError = 2;
		else {
			std::istringstream(argv[4]) >> encOpt;
			if ((encOpt > 9) || (encOpt < 0))
				// Return the value
				isError = 1;
			else {
				outFile = (std::string)argv[2];
				std::string processFile = "";
				std::ifstream listFile;
				listFile.open(argv[1], std::ios::binary);
				while(getline(listFile, processFile)) {
					std::string file = processFile.substr(0, processFile.find(';'));
					if (fileExists(file.c_str()))
						inFileList.push_back(processFile);
					else
						nonExistentFiles.push_back(processFile);
					processFile.clear();
				}
				listFile.close();
				if (!inFileList.size()) // No file exists
					isError = 3;
			}
		}
	} else
		isError = 1; 
	
	// If no error, reported go from here
	if (!isError) {
		// Create the vector to hold encoded data
		std::vector<std::string> encodedData;
		std::string encodedFileNames = "";
		// Check for the size of the inFileList. If there is a size
		// the user opted for multiple files combined and encoded
		// into one; if there is no size, the user opted for one
		// file encoded
		int sizeOfInFileList = inFileList.size();
		
		if (sizeOfInFileList) {
			for (int i = 0; i < sizeOfInFileList; i++) {
				int sizeOfFileName = inFileList[i].size();
				std::string file = inFileList[i].substr(0, (sizeOfFileName - 1));
				std::string datum = getFileBinary(file.c_str(), encOpt);
				if (datum != "false") {
					encodedData.push_back(datum);
					std::string binFileName = "";
					for (int x = 0; x < sizeOfFileName; x++) {
						unsigned char c = inFileList[i][x];
						unsigned int i = c;
						if ((encOpt == 1) ||
							(encOpt == 0))
							binFileName += getBinary(c);
							// printBinary(c, out);
						else if (encOpt == 2)
							// TODO: Add config entry and get config entry values
							binFileName += getBinary(i + 314);
						else if (encOpt == 3)
							// TODO: Add config entry and get config entry values
							binFileName += getBinary((int)c + 314);
							// printBinary(((int)c + 314), out);
					}
					encodedFileNames += encodeBinaryString(binFileName, encOpt);
				}
			}
		} else
			encodedData.push_back(getFileBinary(inFile.c_str(), encOpt));
		
		// Open the output file stream
		ofstream out;
		out.open(outFile.c_str(), std::ios::binary);
		
		if (!out.is_open()) {
			// Close in file
			out.close();
			
			// Return the value
			return 3;
		} else {
			if (sizeOfInFileList) {
				int sizeOfEncodedData = encodedData.size();
				out << encodedFileNames << endl;
				for (int inc = 0; inc < sizeOfEncodedData; inc++) {
					std::vector<std::string> txt = encodeBinaryStringList(encodedData[inc], encOpt);
					int sizeOfTxt = txt.size();
					for (int i = 0; i < sizeOfTxt; i++) {
						out << txt[i];
					}
					out << endl;
				}
			} else {
				std::vector<std::string> txt = encodeBinaryStringList(encodedData[0], encOpt);
				int sizeOfTxt = txt.size();
				for (int i = 0; i < sizeOfTxt; i++) {
					out << txt[i];
				}
			}
		}
		
		// Close out file
		out.close();

		// Decode the file for the selected value
		std::vector<std::string> decoded = decodeBinaryFile(outFile.c_str(), encOpt);
		std::vector<std::string> files;
		int filePos = -1;
		int pos = 0;
		std::string::size_type semi = decoded[0].find(';');
		while (semi != std::string::npos) {
			std::string file = decoded[0].substr(pos, semi - pos);
			files.push_back(file);
			pos = (semi + 1);
			semi = decoded[0].find(';', pos);
			if (file == "../include/Writing.h")
				filePos = files.size();
		}
		if (filePos != -1) {
			//cout << filePos << endl;
			cout << decoded[filePos];
		} else {
			// Decode all if the file not found, because there is
			// only one file
			std::ofstream vid;
			vid.open("wow.3gp", std::ios::binary);
			int sizeOfDecoded = decoded.size();
			for (int i = 0; i < sizeOfDecoded; i++) {
				vid << decoded[i];
			}
			vid.close();
		}
	}
	cout << isError;
	// Return the value
	return isError;
}























/*
// CORRECT CODE
if (isEncoded == 2)
	ch = (char)(int)charNum;
else if (isEncoded == 3)
	ch = ((char)((int)charNum - 314));
*/