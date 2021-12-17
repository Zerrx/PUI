#ifndef WINDOWSCONSOLE_H
#define WINDOWSCONSOLE_H

#include <vector>
#include <string>

//////////////////////////////////
//     WINDOWS      WINDOWS     //
//  PROPER CLEAR SCREEN METHOD  //
//////////////////////////////////
void ClearScreen(void);

void ClearScreenBackTo(int column, int row);


//////////////////////////////
// Getting the Screen Width //
//////////////////////////////
int getWindowsConsoleWidth(void);


///////////////////////////////////
//  MOVE THE CURSOR TO POSITION  //
///////////////////////////////////
void MoveCursorBackTo(int column, int row);

void GoToXY(int column, int line);

int GetX(void);

int GetY(void);

int GetColumn(void);

int GetRow(void);


/////////////////////////////////////
//  GET CONTENTS FROM THE CONSOLE  //
/////////////////////////////////////
std::vector<std::string> GetConsoleContents(
		const int COLUMNS, const int ROWS);

#endif