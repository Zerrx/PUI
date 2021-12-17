#ifndef PRINTBINARY_H
#define PRINTBINARY_H
// From Thinking in C++, 2nd Edition
// (c) Bruce Eckel 2000

// Regular includes
#include <fstream>

void printBinary(const unsigned char val);

void printBinary(const unsigned char val, std::ofstream &out);

std::string getBinary(const unsigned char val);

#endif
