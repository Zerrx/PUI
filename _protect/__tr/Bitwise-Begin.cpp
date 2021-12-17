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

using namespace std;

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
			if ((c == '\r') || (c == '\n')) {
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
	// Encoding option: possible values 0-9
	int encOpt = 0;
	// Use a string variable for file names in case we need to
	// do work with the file names
	std::string inFile = "i";
	std::string outFile = "INTRODUCTIONS";
	if (argc == 4) {
		std::istringstream(argv[3]) >> encOpt;
		if ((encOpt > 9) || (encOpt < 0))
			// Return the value
			return 1;
		inFile = (std::string)argv[1];
		outFile = (std::string)argv[2];
	} else if (argc == 2)
		std::istringstream(argv[1]) >> encOpt;
	
	// Open an input file stream
	ifstream in;
	in.open(inFile.c_str());
	
	std::string bin = "";
	
	if (!in.is_open()) {
		// Close in file
		in.close();
		
		// Return the value
		return 2;
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
	
	// Open an output file stream
	ofstream out;
	out.open(outFile.c_str());
	
	if (!out.is_open()) {
		// Close in file
		out.close();
		
		// Return the value
		return 3;
	} else {
		std::vector<std::string> txt = encodeBinaryStringList(bin, encOpt);
		int sizeOfTxt = txt.size();
		for (int i = 0; i < sizeOfTxt; i++) {
			out << txt[i];
		}
	}
	
	// Close out file
	out.close();

	std::vector<std::string> decoded = decodeBinaryFile(outFile.c_str(), encOpt);
	int sizeOfDecoded = decoded.size();
	for (int i = 0; i < sizeOfDecoded; i++) {
		cout << decoded[i] << endl;
	}
	
	// Return the value
	return 0;
}























/*
// CORRECT CODE
if (isEncoded == 2)
	ch = (char)(int)charNum;
else if (isEncoded == 3)
	ch = ((char)((int)charNum - 314));
*/