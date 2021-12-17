//: C03:Bitwise.cpp
// From Thinking in C++, 2nd Edition
// Available at http://www.BruceEckel.com
// (c) Bruce Eckel 2000
// Copyright notice in Copyright.txt
//{L} printBinary
// Demonstration of bit manipulation

#include "printBinary.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

using namespace std;

void PR(unsigned char a, ofstream &out) {
	printBinary(a, out);
}

std::vector<std::string> getTxtFromBinStrFile(const char *fileName, int isEncoded) {
	std::vector<std::string> txt;
	std::ifstream in;
	in.open(fileName);
	
	if (in.is_open()) {
		while (true) {
			if (in.tellg() == EOF)
				break;
			std::string dataLine = "";
			std::string binaryLine;
			getline(in, binaryLine);
			int sizeOfBinaryLine = binaryLine.size();
			int i = 0;
			int next = 80;
			while (i < sizeOfBinaryLine) {
				std::string byte = binaryLine.substr(i, 8);
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
				// Default 0,1
				unsigned char ch = (char)charNum;
				if (isEncoded == 1)
					ch = (char)(int)charNum;
				else if (isEncoded == 2)
					ch = ((unsigned char)((int)charNum - 314));
				/*
				// CORRECT CODE
				if (isEncoded == 2)
					ch = (char)(int)charNum;
				else if (isEncoded == 3)
					ch = ((char)((int)charNum - 314));
				*/
				if (((i + 8) == next) ||
					((i + 8) == sizeOfBinaryLine)){
					dataLine += ch;
					txt.push_back(dataLine);
					dataLine.clear();
					next += 80;
				} else
					dataLine += ch;
				i += 8;
			}
		}
	}
	
	in.close();
	
	// Return the value
	return txt;
}

std::vector<std::string> decodeString(const char *fileName, int isEncoded) {
	std::vector<std::string> txt;
	std::ifstream in;
	in.open(fileName, std::ios::binary);
	std::string tmp;
	if (in.is_open()) {
		while (true) {
			if (in.tellg() == EOF)
				break;
			//int ch = (char)in.get();
			std::string s = getBinary(((int)((unsigned char)in.get())));
			//((unsigned char)((int)charNum))
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
			charNum += 314;
			cout << (unsigned char)(unsigned int)charNum;
		}
		txt.push_back(tmp);
		//cout << tmp;
	}
	
	in.close();
	
	// Return the value
	return txt;
}

int main(int argc, char **argv) {
	int encOpt;
	if (argc > 1)
		std::istringstream(argv[1]) >> encOpt;
	else
		encOpt = 0;
	std::vector<std::string> txt111 = decodeString("bkd.txt", encOpt);
	//for (int i = 0; i < txt.size(); i++) {
		//cout << txt[i] << endl;
	//}
	
	
	//Open an input and output stream in binary mode
	ifstream in;
	ofstream out;
	in.open("i");
	out.open("INTRODUCTIONS");
	
	if (in.is_open()) {
		while (!in.eof()) {
			unsigned char c = in.get();
			unsigned int i = c;
			if ((encOpt == 1) ||
				(encOpt == 0))
				PR(c, out);
			else if (encOpt == 2)
				PR(i, out);
			else if (encOpt == 3)
				PR(((int)c + 314), out);
		}
	}
	
	in.close();
	out.close();
	
	out.open("bkd.txt");
	std::vector<std::string> txt = getTxtFromBinStrFile("INTRODUCTIONS", encOpt);
	for (int i = 0; i < txt.size(); i++) {
		out << txt[i];
	}
	// Close last out file
	out.close();

}