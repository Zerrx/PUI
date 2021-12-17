// Includes
#include <windows.h>
#include <vector>
#include <string>
#include "..\..\include\WindowsConsole.h"


//////////////////////////////////
//  PROPER CLEAR SCREEN METHOD  //
//////////////////////////////////
void ClearScreen(void) {
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = {0,0};
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	
	// Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
		return;
		
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	
	// Fill the entire buffer with spaces
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count))
		return;
	
	// Fill the entire buffer with the current
	// colors and attributes
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
		return;
	
	// Move the cursor home
	SetConsoleCursorPosition(hStdOut, homeCoords);
}

void ClearScreenBackTo(int column, int row) {
	//////////////////////////////
	// column == X
	//    row == Y
	//////////////////////////////
	
	// Declare the variables to hold the count of cells
	// to "erase", count of cells "erased", and the
	// coordinates we are resetting the cursor to and
	// erasing the contents of the screen after
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = {column, row};
	// Get the output handle
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	// Declare the buffer info variable
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	// Check if we have a valid handle
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;
	
	// Get the number of cells in the current buffer
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
		return;
	
	cellCount = csbi.dwSize.X * csbi.dwSize.Y;
	
	FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count);
	
	// Move the cursor home
	MoveCursorBackTo(column, row);
}


//////////////////////////////
// Getting the Screen Width //
//////////////////////////////
int getWindowsConsoleWidth(void) {
	int windowsConsoleWidth = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if (!GetConsoleScreenBufferInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&csbi))
		windowsConsoleWidth = -1;
	else
		windowsConsoleWidth = csbi.dwSize.X;
	
	// Return the value
	return windowsConsoleWidth;
}


///////////////////////////////////
//  MOVE THE CURSOR TO POSITION  //
// MANY SPECIAL THANKS TO Duoas  //
///////////////////////////////////
void MoveCursorBackTo(int column, int row) {
	COORD homeCoords = {column,row};
	
	// Move the cursor home
	SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			homeCoords);
}

void GoToXY(int column, int line) {
	COORD coordinate;
	coordinate.X = column;
	coordinate.Y = line;
	
	SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			coordinate);
}

int GetX(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	
	return csbi.dwCursorPosition.X;
}

int GetY(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	
	return csbi.dwCursorPosition.Y;
}

int GetColumn(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	
	return csbi.dwCursorPosition.X;
}

int GetRow(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	if(!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return -1;
	
	return csbi.dwCursorPosition.Y;
}


/////////////////////////////////////
//  GET CONTENTS FROM THE CONSOLE  //
/////////////////////////////////////
std::vector<std::string> GetConsoleContents(const int COLUMNS, const int ROWS) {
	HANDLE hStdout;
	SMALL_RECT srctReadRect;
	CHAR_INFO chiBuffer[COLUMNS * ROWS];
	COORD coordBufSize;
	COORD coordBufCoord;
	BOOL fSuccess;
	std::vector<std::string> consoleData;
	
	// Get a handle to the STDOUT screen buffer to copy from and
	// create a new screen buffer to copy to
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//if (hStdout == INVALID_HANDLE_VALUE)
	//	return 1;
	
	// Set the source rectangle
	srctReadRect.Top = 0; // top left: row 0, col 0
	srctReadRect.Left = 0;
	srctReadRect.Bottom = (ROWS - 1); // bottom right: row 1, col 79
	srctReadRect.Right = (COLUMNS - 1);
	
	// The temporary buffer size is COLUMNS columns X ROWS rows
	coordBufSize.X = COLUMNS;
	coordBufSize.Y = ROWS;
	
	// The top left destination cell of the temporary buffer is
	// row 0, col 0
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;
	
	// Copy the block from the screen buffer to the temporary buffer
	fSuccess = ReadConsoleOutput(
			hStdout, // screen buffer to read from
			chiBuffer, // buffer to copy into
			coordBufSize, // col-row size of chiBuffer
			coordBufCoord, // top-left destination cell in chiBuffer
			&srctReadRect); // screen buffer source rectangle
			
	// Temporary string to hold data line by line
	std::string tmpLineData = "";
	// Temporary line counter
	int tmpLineCount = 1;
	for (int i = 0; i < (COLUMNS * ROWS); i++) {
		if (i < (tmpLineCount * COLUMNS))
			tmpLineData += chiBuffer[i].Char.AsciiChar;
		if (i == ((tmpLineCount * COLUMNS) - 1)) {
			tmpLineCount++;
			consoleData.push_back(tmpLineData);
			tmpLineData.clear();
		}
	}
	
	// Return the value
	return consoleData;
}