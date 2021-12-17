#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;

int main(int argc, char *argv[]) {
	//Open an input and output stream in binary mode
	ifstream in("bin.png", ios::binary);
	//ifstream in("myimage.jpg", ios::binary);
	ofstream out("myimage1.png", ios::binary);

	//if (in.is_open() && out.is_open()) {
	if (in.is_open()) {
		while (!in.eof()) {
			out<<in.get();
		} 
	}
	//Close both files
	in.close();
	out.close();
	system("pause");
}