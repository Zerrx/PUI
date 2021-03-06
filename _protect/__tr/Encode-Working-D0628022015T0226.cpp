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
		while (!in.eof()) {
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
				char ch;// = (char)charNum;
				if (isEncoded == 1)
					ch = (char)(int)charNum;
				else if (isEncoded == 2)
					ch = ((unsigned char)((int)charNum));
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
			charNum -= 314;
			cout << (unsigned char)(unsigned int)charNum;
//			cout << (char)((int)in.get() + 314);
//(unsigned char)(int)in.get() + 314;
//			unsigned int i = (int)(char)in.get() + 314;
//			unsigned char c = i;
//			tmp += c;
			//tmp += (unsigned char)(unsigned int)(in.get() + 314);
			//tmp += (unsigned char)(unsigned int)(((int)(char)in.get()) + 314);
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
				PR(i+314, out);
			else if (encOpt == 3)
				PR(((int)c + 314), out);
		}
	}
	
	in.close();
	out.close();
	
	std::string blick = "What [AND] a winterfulG G|\"wDeather; asdlfka\nlinking together textual datas stuff stream;ing like this!**&^`~";
	std::string bin = "";
	std::string enc = "";
	for (int aaa = 0; aaa < blick.size(); aaa++) {
		unsigned char b = blick[aaa];
		unsigned int i = b;//116 difference
		bin += getBinary(i, true);
	}
	for (int bbb = 0; bbb < bin.size();) {
		std::string t = bin.substr(bbb, 8);
		int charNum = 0;
		int exponent = -1;
		for (int j = 7; j >= 0; j--) {
			// Increment the exponent as we iterate through
			// the byte string to raise the power according
			// to binary rules for adding
			exponent++;
			// Convert ASCII num to int num
			int num = (t[j] - '0');
			charNum += ((num == 1) ? std::pow(2, exponent) : 0);
		}
		char ch = (char)charNum;
		if (encOpt == 2)
			ch = ((char)((int)charNum - 314));
		enc += ch;
		bbb += 8;
	}
	std::ofstream o;
	o.open("pin.txt");
	o << enc << endl;
	o.close();
	std::ifstream i;
	i.open("pin.txt");
	std::string blah = "";
	std::string tMa;
	while (true) {
		if (i.tellg() == EOF)
			break;
		blah += (unsigned char)(int)i.get() + 314;
	}
	i.close();
	cout << endl << blah;
	
	out.open("bkd.txt");
	std::vector<std::string> txt = getTxtFromBinStrFile("INTRODUCTIONS", encOpt);
	for (int i = 0; i < txt.size(); i++) {
		out << txt[i];
	}
	// Close last out file
	out.close();

}










/*
		bool al = false;
					if (!al) {
						std::cout << std::boolalpha << (num == 1) << endl;
						std::cout << j << "    " << std::pow(2, exponent) << endl;
					}

				if (!al) {
					std::cout << byte << endl;
					std::cout << charNum << endl;
					std::cout << (char)charNum << endl;
					al = true;
				}

d





// printBinary(a);
//	std::cout << " " << std::endl;

//	char getval;
//	unsigned char a;
//	cout << "Enter a number between 0 and 255: ";
//	cin >> getval; a = getval;
//	PR(a);
//	unsigned char b = ' ';
//	PR(b);
switch (j) {
	case 7:
		if (z == 1)
			q += 1;
		break;
	case 6:
		if (z == 1)
			q += 2;
		break;
	case 5:
		if (z == 1)
			q += 4;
		break;
	case 4:
		if (z == 1)
			q += 8;
		break;
	case 3:
		if (z == 1)
			q += 16;
		break;
	case 2:
		if (z == 1)
			q += 32;
		break;
	case 1:
		if (z == 1)
			q += 64;
		break;
	case 0:
		if (z == 1)
			q += 128;
		break;
}
}
if (!doOnce && j>=0) {
	std::cout << j << endl;
	std::cout << (int)z << endl;
	std::cout << std::boolalpha << (z == 1) << endl;
	if (j == 0) {
		doOnce = true;
	}
}
if (!doOnce){doOnce=true;}
else if (!doOnce2) {
	std::cout << endl << endl << "Brett q is: " << q << endl << endl << (char)q;
	doOnce2 = true;
}
*/












//		cout << "\n\n\n\n\n\n\n\n" << tmp.size() << "\n\n\n\n\n\n\n\n";
//		for (int bbb = 0; bbb < tmp.size(); bbb++) {
			//int bin = in.get();
			// cast to char temporary
			
//			char bl = tmp[bbb];
			//char bl = in.get();
			//std::string bl = getBinary(tmp[bbb]);
			//int charNum = 0;
			//int exponent = -1;
			//for (int j = 7; j >= 0; j--) {
				// Increment the exponent as we iterate through
				// the byte string to raise the power according
				// to binary rules for adding
				//exponent++;
				// Convert ASCII num to int num
				//int num = (bl[j] - '0');
				//charNum += ((num == 1) ? std::pow(2, exponent) : 0);
			//}
			//char c = charNum;
//			int b = c;
			//int b = c + 314;
			//unsigned char y = (unsigned int)b;
			//int bin = ((int)c + 314);
	//cout << y << endl;
//	unsigned char bl = in.get();
//	if (bl == '\n' || bl == '\r')
//		continue;
//	cout << (char)(unsigned char)(unsigned int)(bl + 314);
	//cout << (char)((unsigned char)(int)bl + 314);
	//cout << (unsigned char)((int)in.get() + 314);
//			char ch = ((char)((int)charNum - 314));
//			unsigned int i = char;
			
			//char c = (char)in.get();
			//int bin = ((int)c + 314);
	//cout << c << endl;
			
//			unsigned char b = 'X';
//			unsigned int i = ((unsigned int)b - 314);//116 difference
//			char ch = (char)i;
//			cout << endl << endl << endl << b << "    " << (int)b << "    " << i << endl <<
//					ch << "    " << (int)ch << "    " << (unsigned char)((int)ch + 314);
			
			
			//cout << (char)(unsigned char)(unsigned int)(bin + 314);
			//cout << (unsigned char)(unsigned int)bin;
//				char ch = (char)num;
//				if (isEncoded == 1)
//					ch = (char)(int)num;
//				else if (isEncoded == 2)
//					ch = ((char)((int)num + 314));
//				cout << ch;
				//if ((ch == '\n') ||
				//		(ch == '\r')) {
				//	txt.push_back(dataLine);
				//	dataLine.clear();
				//} else
				//	dataLine += ch;
			//	num = 0;
			//	power = 7;
			//}
		//}




// A macro to save typing:
//#define PR(STR, EXPR) cout << STR; printBinary(EXPR); cout << endl;  
