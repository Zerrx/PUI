// From Thinking in C++, 2nd Edition
// (c) Bruce Eckel 2000

// Regular includes
#include <iostream>
#include <fstream>
#include <string>
// Application includes
#include "printBinary.h"

void printBinary(const unsigned char val) {
	for(int i = 7; i >= 0; i--) {
		if(val & (1 << i))
			std::cout << "1";
		else
			std::cout << "0";
	}
}

void printBinary(const unsigned char val, std::ofstream &out) {
	for(int i = 7; i >= 0; i--) {
		if(val & (1 << i))
			out << "1";
		else
			out << "0";
	}
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
