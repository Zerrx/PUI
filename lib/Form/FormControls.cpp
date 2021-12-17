////////////////////////////////////////////
// Form Controls
////////////////////////////////////////////

// Regular includes
#include <cstdio>
// Application includes
#include "..\..\include\WindowsConsole.h"
#include "..\..\include\FormControls.h"

void populateData(int columnNumber,
		int lineNumber,
		int fieldSize,
		std::string data,
		int InputControl[2]) {
	// lineNumber is the human-understood screen position,
	// since positions on the screen begin at computer-
	// understood 0, lineNumber must be decremented
	lineNumber--;
	// Need the size of the data
	int dataSize = data.size();
	// NOTE::::::::::::::::::::::::::::::::::::::::::::::
	// Need the amount of data to output to output spaces
	// in case the data we are writing is less than the
	// initial __________ used for visual representation
	// of the field the user is currently "editing"
	// USING fieldSize FOR THE ABOVE NOTE ^^
	// END NOTE::::::::::::::::::::::::::::::::::::::::::
	bool needsSpaces = false;
	// Check if the size of the data is greater than the
	// available fieldSize; if it is, then we need to
	// limit the output to the size of the field; if the
	// dataSize is less than the fieldSize, then we need
	// to output spaces for the possibility of the above
	// NOTE occurring.
	if (dataSize > fieldSize)
		dataSize = fieldSize;
	else
		needsSpaces = true;
	// Go to the field position
	MoveCursorBackTo(columnNumber, lineNumber);
	int i = 0;
	for (i = 0; i < dataSize; i++)
		putchar(data[i]);
	
	if (needsSpaces) {
		for (i; i <= fieldSize; i++) {
			putchar(' ');
		}
	}
	// Clear the screen from the input control area on
	// so that we get the next data and not have any
	// previously written data visible in our form
	// input field area
	ClearScreenBackTo(InputControl[0],InputControl[1]);
}
