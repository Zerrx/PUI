// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
// Regular includes
#include <ctime>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\..\include\conf.h"
#include "..\..\..\include\DateTime.h"
#include "..\..\..\include\FileIO.h"
#include "..\..\..\include\FormControls.h"
#include "..\..\..\include\Interface.h"
#include "..\..\..\include\PUIMath.h"
#include "..\..\..\include\WindowsConsole.h"
#include "..\..\..\include\ShellControls.h"

// Namespaces
using namespace std;
using namespace PUI;

bool StairStringer() {
	// Initialize a userWantsToClose boolean
	bool userWantsToClose = false;
	// Get the console width
	int consoleWidth = getWindowsConsoleWidth();
	// Declare the variables that will hold user data
	std::string personName = "";
	std::string calculationDate = "D";
	std::string address = "";
	std::string locationOnProperty = "";
	std::string cityStateZip = "";
	std::string stairStringerDirectory = config.getValue("Module.Construction.StairStringer.Directory");
	// Currently, the totalRise is the only variable
	// other than name, address, location, and C/S/Z,
	// that will hold user input
	std::string totalRiseStr = "";
	double totalRise = 0;
	double approximateNumberOfSteps = 0;
	int numberOfSteps = 0;
	double stepRiser = 0;
	double treadWidth = 0;
	double totalRun = 0;
	double inchRise = 0;
	double stepIncline = 0;
	double stringerIncline = 0;
	// Declare the output file path variable
	std::string fileOut = (stairStringerDirectory + "Stringer-");
	// Need to know the form width, so declare a variable to hold
	// that information which will be set later
	int formWidth = 0;
	// Declare the array that will hold the input/control location
	int InputControl[2]; // 2 items X-pos and Y-pos
	
	// Get the current Date/Time
	struct tm currentDateTime = GetCurrentDateTM();
	struct DayAndWeekInfo currentDayWeek;
	struct PUIVisibleDTS theCurrentDay;
	currentDayWeek = getDayAndWeekInfo(currentDateTime);
	theCurrentDay = currentDayWeek.dayDTI;
	calculationDate += (theCurrentDay.pui_gdt_str_int_wday +
			theCurrentDay.pui_gdt_str_mday +
			theCurrentDay.pui_gdt_str_int_mon +
			theCurrentDay.pui_gdt_str_year + "T" +
			theCurrentDay.pui_gdt_str_hour +
			theCurrentDay.pui_gdt_str_min);
	
	// Clear the screen
	ClearScreen();
	
	// Get the introduction
	getIntroduction("stairstringer");
	
	// Get a handle to user input so that we can erase
	// the welcome message and populate the screen with
	// the STAIR_STRINGER_DISPLAY data
	while (true) {
		std::string getReady;
		std::cout << std::endl << std::endl << ">  ";
		getline(std::cin, getReady);
		if (isClose(getReady)) {
			userWantsToClose = true;
			break;
		} else if (getReady == "r")
			break;
		else
			cout << "\nInvalid input...\n\n";
	}
	
	if (!userWantsToClose) {
		// Clear the screen
		ClearScreen();
		
		// Populate the screen with the data from the
		// STAIR_STRINGER_DISPLAY file to get data
		// from the user and inform them of the field
		// they are currently working in by using
		// "underlines" as field markers
		std::string fileDisp = "C:\\Env\\Dev\\PUI\\app\\PUI\\categories\\construction\\StairStringer\\STAIR_STRINGER_DISPLAY";
		std::ifstream *stringerDisplay = readFile(fileDisp.c_str());
		// Output the data to the screen
		while (!stringerDisplay->eof()) {
			std::string dData;
			getline(*stringerDisplay, dData);
			if (!formWidth)
				formWidth = dData.size();
			if (consoleWidth > 80)
				std::cout << dData << std::endl;
			else
				std::cout << dData;
		}
		closeReadFile(stringerDisplay);
		// Get the end of the form position!
		InputControl[0] = GetX();
		InputControl[1] = GetY();
		// Populate the date
		populateData(53, 30, 25, calculationDate, InputControl);//19,53,23,63
	}
	
	// Check if valid input for field has been met
	bool isValidPerson = false;
	bool isValidAddress = false;
	bool isValidLocation = false;
	bool isValidCSZ = false;
	bool isValidTotalRise = false;
	 
	// Get the file information such as the person's
	// name, the date of stringer form construction,
	// the address of the work to be done and their
	// city/state/zip information, as well as the
	// location on the premises where the stringer(s)
	// will be installed
	// Get the field's data
	while (!userWantsToClose) {
		if (!isValidPerson) {
			// Populate the form with input data or
			// the field marker "________"
			// Do this ^^ for all form fields with
			// user input controls
			personName = "________";
			populateData(19, 30, 27, personName, InputControl);//19,53,23,63
			std::cout << std::endl << std::endl << ">  ";
			getline(std::cin, personName);
			if (isClose(personName)) {
				userWantsToClose = true;
				break;
			} else if (personName == "") {
				std::cout << "\nInput required...\n\n";
				continue;
			} else {
				isValidPerson = true;
				populateData(19, 30, 27, personName, InputControl);
			}
		}
		
		if (!isValidAddress) {
			// Populate the visible and file forms
			// with the input data
			address = "________";
			populateData(19, 31, 23, address, InputControl);//19,53,23,63
			std::cout << std::endl << std::endl << ">  ";
			getline(std::cin, address);
			if (isClose(address)) {
				userWantsToClose = true;
				break;
			} else if (address == "") {
				cout << "\nInput required...\n\n";
				continue;
			} else {
				isValidAddress = true;
				populateData(19, 31, 23, address, InputControl);//19,53,23,63
			}
		}
		
		if (!isValidLocation) {
			// Populate the visible and file forms
			// with the input data
			locationOnProperty = "________";
			populateData(53, 31, 25, locationOnProperty, InputControl);//19,53,23,63
			std::cout << std::endl << std::endl << ">  ";
			getline(std::cin, locationOnProperty);
			if (isClose(locationOnProperty)) {
				userWantsToClose = true;
				break;
			} else if (locationOnProperty == "") {
				cout << "\nInput required...\n\n";
				continue;
			} else {
				isValidLocation = true;
				populateData(53, 31, 25, locationOnProperty, InputControl);//19,53,23,63
			}
		}
		
		if (!isValidCSZ) {
			// Populate the visible and file forms
			// with the input data
			cityStateZip = "________";
			populateData(19, 32, 59, cityStateZip, InputControl);//19,53,23,63
			std::cout << std::endl << std::endl << ">  ";
			getline(std::cin, cityStateZip);
			if (isClose(cityStateZip)) {
				userWantsToClose = true;
				break;
			} else if (cityStateZip == "") {
				cout << "\nInput required...\n\n";
				continue;
			} else {
				isValidCSZ = true;
				populateData(19, 32, 59, cityStateZip, InputControl);//19,53,23,63
			}
		}
		
		if (!isValidTotalRise) {
			// Populate the visible and file forms
			// with the input data
			totalRiseStr = "________";
			populateData(23, 34, 17, totalRiseStr, InputControl);//19,53,23,63
			std::cout << std::endl << std::endl << ">  ";
			getline(std::cin, totalRiseStr);
			if (isClose(totalRiseStr)) {
				userWantsToClose = true;
				break;
			} else if (totalRiseStr == "") {
				cout << "\nInput required...\n\n";
				continue;
			} else {
				std::istringstream(totalRiseStr) >> totalRise;
				if (!(totalRise > 20)) {
					cout << "\nInvalid input...20 inches or greater!\n\n";
					continue;
				}
				populateData(23, 34, 17, convertDoubleToString(totalRise), InputControl);//19,53,23,63
				approximateNumberOfSteps = (totalRise / 7.0);
				populateData(63, 34, 15, getNumberWithPreciseDecimalPlaces(2, approximateNumberOfSteps), InputControl);//19,53,23,63
				numberOfSteps = approximateNumberOfSteps;
				populateData(23, 35, 21, convertIntToString(numberOfSteps), InputControl);//19,53,23,63
				stepRiser = (totalRise / numberOfSteps);
				populateData(63, 35, 15, getNumberWithPreciseDecimalPlaces(2, stepRiser), InputControl);//19,53,23,63
				treadWidth = (17.5 - stepRiser);
				populateData(23, 36, 28, getNumberWithPreciseDecimalPlaces(2, treadWidth), InputControl);//19,53,23,63
				totalRun = (treadWidth * (numberOfSteps - 1));
				populateData(63, 36, 15, getNumberWithPreciseDecimalPlaces(2, totalRun), InputControl);//19,53,23,63
				inchRise = ((stepRiser * 12) / treadWidth);
				populateData(23, 37, 25, getNumberWithPreciseDecimalPlaces(2, inchRise), InputControl);//19,53,23,63
				stepIncline = sqrt((pow(stepRiser, 2) + pow(treadWidth, 2)));
				populateData(63, 37, 15, getNumberWithPreciseDecimalPlaces(3, stepIncline), InputControl);//19,53,23,63
				stringerIncline = (stepIncline * (numberOfSteps - 1));
				populateData(23, 38, 55, getNumberWithPreciseDecimalPlaces(3, stringerIncline), InputControl);//19,53,23,63
				break;
			}
		}
	}
	
	if (!userWantsToClose) {
		fileOut += (personName + "-" + calculationDate + ".txt");
		std::ofstream stringer;
		stringer.open(fileOut.c_str());
		
		// Get the data on the screen
		std::vector<std::string> stringerData = GetConsoleContents(consoleWidth, InputControl[1]);
		// Get the size of the data on the screen (in rows)
		int stringerDataSize = stringerData.size();
		
		// Output the data to the file
		for (int i = 0; i < stringerDataSize; i++)
			stringer << stringerData[i] << std::endl;
		
		// In case some text was too long and truncated
		// in the form, dump the text beneath the form
		stringer << std::endl << std::endl << std::endl;
		stringer << "[FULL_FIELD_INPUT][LINE_COUNT:14]" << std::endl;
		stringer << std::setw(23) << "Name: " << personName << std::endl;
		stringer << std::setw(23) << "Date of Calculation: " << calculationDate << std::endl;
		stringer << std::setw(23) << "Address: " << address << std::endl;
		stringer << std::setw(23) << "Location: " << locationOnProperty << std::endl;
		stringer << std::setw(23) << "City/State/Zip: " << cityStateZip << std::endl;
		stringer << std::setw(23) << "Total Rise: " << totalRise << std::endl;
		stringer << std::setw(23) << "Approx. NO. of Steps: " << approximateNumberOfSteps << std::endl;
		stringer << std::setw(23) << "Actual NO. of Steps: " << numberOfSteps << std::endl;
		stringer << std::setw(23) << "Step Riser: " << stepRiser << std::endl;
		stringer << std::setw(23) << "Tread Width: " << treadWidth << std::endl;
		stringer << std::setw(23) << "Total Run: " << totalRun << std::endl;
		stringer << std::setw(23) << "Inch Rise: " << inchRise << std::endl;
		stringer << std::setw(23) << "Step Incline: " << stepIncline << std::endl;
		stringer << std::setw(23) << "String Incline: " << stringerIncline << std::endl;
		
		// Close the output file
		stringer.close();
		
		// Pause to show the user the results of the
		// necessary calculations
		Pause(true);
	}

	std::cout << "Closing the application now!\n\n";
	
	// Return the value
	return userWantsToClose;
}



///////////////////////////////////////////////////////
///////// TO COMPILE: Make int main() and run
///////// 		g++ -o stringer.exe StairStringer.cpp ../../../lib/DateTime/DateTime.cpp ../../../lib/ShellControls/WindowsConsole.cpp ../../../lib/ShellControls/ShellControls.cpp ../../../lib/ShellControls/WindowsClearScreen.cpp
///////////////////////////////////////////////////////
