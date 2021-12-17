// Function Prototypes
// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
// Regular includes
#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <limits>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\..\include\conf.h"
#include "..\..\..\include\CreateShorthandRoutine.h"
#include "..\..\..\include\DateTime.h"
#include "..\..\..\include\FileIO.h"
#include "..\..\..\include\FileSystem.h"
#include "..\..\..\include\FormControls.h"
#include "..\..\..\include\Interface.h"
#include "..\..\..\include\ShellControls.h"
#include "..\..\..\include\WindowsConsole.h"
#include "..\..\..\include\Writing.h"

using namespace std;

using namespace PUI;

bool isTesting = false;
bool CreateShorthandRoutine() {
	// Clear the screen
	ClearScreen();
	
	// Check for close at any input point...thus,
	// if on inputting the end time, set a boolean
	// isCanceled to true and do nothing else
	// Initialize the isCanceled boolean
	// Initialize the isNotQuit boolean
	bool isNotQuit = true;
	
	// Get the introduction
	getIntroduction("shorthandroutine");
	
	// Prompt user for routine Date/verification
	struct PUIVisibleDTS routineDate = getDate();
	if (routineDate.pui_gdt_week != -2) {
		// Set up the variables
		std::string routineTime;
		std::vector<std::string> shorthandData;
		std::vector<std::string> shorthandFileInfo;
		std::string filePath;
		
		// Get and validate the routine begin time
		while (true) {
			std::string rBT;
			std::cout << "\nPlease enter the begin time of the routine.\n\n> ";
			getline(std::cin, rBT);
			if (isClose(rBT)) {
				isNotQuit = false;
				break;
			} else if (isValidTime(rBT)) {
				routineTime = ("(" + getTimePeriod(rBT) +
						": " + rBT + " hrs" + "; ");
				break;
			} else
				std::cout << "\nThe time entered is not valid!\n";
		}
		
		// If no element present, then no cancellation notice
		// was given, we can continue
		if (isNotQuit) {
			if (isTesting) {
				std::string bodybuildingDirectory = config.getValue("Module.Sports.Bodybuilding.Directory");
				std::string gePath = (bodybuildingDirectory + "Routine Information\\Weekly\\Week 2015-06\\Shorthand\\__dump__\\10 February 2015.dmp");
				std::ifstream *testData = 0;
				testData = new std::ifstream(gePath.c_str());
				bool readyData = false;
				while (!testData->eof()) {
					std::string line;
					getline(*testData, line);
					if (line == "[SHORTHAND_DATA]")
						readyData = true;
					else if (readyData) {
						if (line != "")
							shorthandData.push_back(line);
					}
				}
				closeReadFile(testData);
			} else // Pass necessary data for help items
				shorthandData = getShorthandData();
			
			if (shorthandData[0] != "-1") {
				// Get and validate the routine end time
				while (true) {
					std::string rET;
					std::cout << "\nPlease enter the end time of the routine.\n\n> ";
					getline(std::cin, rET);
					if (isClose(rET)) {
						isNotQuit = false;
						break;
					} else if (isValidTime(rET)) {
						routineTime += ("End: " + rET + " hrs)");
						break;
					} else
						std::cout << "\nThe time entered is not valid!\n";
				}
				
				// If the user has not canceled, finish
				if (isNotQuit) {
					// Create the file and get all relevant information
					shorthandFileInfo = createShorthandFile(routineDate,
							routineTime, shorthandData);
					// Output file information
					std::cout << std::endl << std::endl <<
							"New TXT file name: " <<
							shorthandFileInfo[0] << std::endl <<
							std::endl << "New TXT file path: " <<
							shorthandFileInfo[1] << std::endl <<
							std::endl << "New CSV file name: " <<
							shorthandFileInfo[2] << std::endl <<
							std::endl << "New CSV file path: " <<
							shorthandFileInfo[3] << std::endl <<
							std::endl;
					
					// Pause so the user can see the information
					Pause(true);
				}
			}
		}
	}
	
	
	// Return the value
	return isNotQuit;
}

std::vector<std::string> createShorthandFile(
		struct PUIVisibleDTS routineDate,
		std::string routineTime,
		std::vector<std::string> shorthandData) {
	// ****************************************************
	// Notes:
	//	-- routineDate - used only to create the name of the file and to write the date/time (routineDateTime) in the file data
	//	-- routineTime - used only to write the date/time (routineDateTime) in the file data
	//	-- filePath - the directory to create the new file (fileName)
	//	-- shorthandData - the routine data to write to the file
	// ****************************************************
	
	std::string monthDay = routineDate.pui_gdt_str_mday;
	std::string month = routineDate.pui_gdt_str_mon;
	std::string year = routineDate.pui_gdt_str_year;
	std::string week = routineDate.pui_gdt_str_week;
	std::string rDate = (monthDay + " " + month + " " + year);
	std::string textFileName = (rDate + ".txt");
	std::string csvFileName = (rDate + ".csv");
	std::string weeklyFolderName = ("Week " + year + "-" + week);
	std::string bodybuildingDirectory = config.getValue("Module.Sports.Bodybuilding.Directory");
	std::string filePath = (bodybuildingDirectory + "Routine Information\\Weekly\\" +
			weeklyFolderName + "\\Shorthand\\");
	
	// Initialize variables;
	std::string routineDateTime = (rDate + " " + routineTime);
	std::string fullFilePath = (filePath + textFileName);
	std::string fullCSVFilePath = (filePath + "CSV\\" + csvFileName);
	std::vector<std::string> shorthandFilePathAndName;
	shorthandFilePathAndName.push_back(textFileName);
	shorthandFilePathAndName.push_back(fullFilePath);
	shorthandFilePathAndName.push_back(csvFileName);
	shorthandFilePathAndName.push_back(fullCSVFilePath);
	
	// Since Windows' functions to create directories
	// does not allow creating a directory past the
	// first one that does not exist, we must have
	// Create the directories if they do not exist
	CreateDirectory(filePath.c_str(), NULL);
	CreateDirectory((filePath + "CSV\\").c_str(), NULL);
	CreateDirectory((filePath + "__dump__\\").c_str(), NULL);
	
	// Assign the used data to a data struct and dump it to a file
	struct ShorthandRoutineStruct shorthandRoutineData;
	shorthandRoutineData.textFileName = textFileName;
	shorthandRoutineData.csvFileName = csvFileName;
	shorthandRoutineData.fullFilePath = fullFilePath;
	shorthandRoutineData.fullCSVFilePath = fullCSVFilePath;
	shorthandRoutineData.weeklyFolderName = weeklyFolderName;
	shorthandRoutineData.shorthandData = shorthandData;
	// Dump the data to the data dump file
	dumpShorthandRoutineData((filePath + "__dump__\\" + (rDate + ".dmp")),
			shorthandRoutineData);
	// First, write the data to the human-readable text file
	writeTXTData(fullFilePath, routineDateTime, shorthandData);
	// Next, write the data to the CSV file for importing to Excel
	writeCSVData(fullCSVFilePath, weeklyFolderName,
			routineDateTime, shorthandData);
	// TODO: Warn the user that any data input that has commas, will
	// need to be modified as CSV files will erringly separate text
	// when you use the delimiter
	
	// Return the file information for the user to view...
	// Return the value
	return shorthandFilePathAndName;
}

void writeTXTData(std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData) {
	// Initialize the file handler
	std::ofstream shorthandFile;
	// Open the shorthand file for editing
	shorthandFile.open(fullFilePath.c_str());
	
	// Write the SimpleView data in LBS
	writeSimpleView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, false);
	// Write the SimpleView data in KGS
	writeSimpleView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, true);
	// Write the LongView data in LBS
	writeLongView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, false);
	// Write the LongView data in KGS
	writeLongView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, true);
	// Write the ListColumnView data in LBS
	writeListColumnView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, false);
	// Write the ListColumnView data in KGS
	writeListColumnView(shorthandFile, fullFilePath,
			routineDateTime, shorthandData, true);
	// Write the ListView data in LBS
//	writeListView(shorthandFile, fullFilePath,
//			routineDateTime, shorthandData, false);
	// Write the ListView data in LBS
//	writeListView(shorthandFile, fullFilePath,
//			routineDateTime, shorthandData, true);
	
	// Close the shorthand file when finished editing
	shorthandFile.close();
}

void writeCSVData(std::string fullCSVFilePath,
		std::string weeklyFolderName,
		std::string routineDateTime,
		std::vector<std::string> shorthandData) {
	// Initialize the file handler
	std::ofstream shorthandCSVFile;
	// Open the shorthand CSV file for editing
	shorthandCSVFile.open(fullCSVFilePath.c_str());
	// Write the date and time
	shorthandCSVFile << routineDateTime << ",,,,,,,,,,," << weeklyFolderName << std::endl;
	
	// Get a handle to the routineName so that we can
	// check if the routine is a dumbbell (i.e., two-
	// handed routine)
	std::string routineName;
	// Write the data to the file
	int numOfData = shorthandData.size();
	// To increment through the list
	int inc = 0;
	// Add LBS and KGS data
	while (inc < numOfData) {
		// Need this variable to concatenate and later modify
		// the set data
		std::vector<std::string> timeToWrite;
		std::vector<std::string> setToWrite;
		std::string datum = shorthandData[inc];
		std::string modDatum = datum;
		if (datum.find("[IS_ROUTINE_NAME]") != std::string::npos) {
			shorthandCSVFile << "," << datum.substr(0,
					datum.find("[IS_ROUTINE_NAME]"));
			
			// Populate the routineName so that we can check if
			// the routine is a dumbbell (i.e., two-handed routine)
			routineName = datum;
			// To add the routine information on the same line
			inc++;
			// After incrementing the increment variable, set
			// datum to the new data to get a handle to the
			// set data
			datum = shorthandData[inc];
			modDatum = datum;
		} else
			shorthandCSVFile << ",";
		std::vector<int> setArr = getValidSetList(modDatum);
		// If the user has entered (and probably will!) the
		// items individually, we need to compress them into
		// one line item for "Simple View"
		// Init the next position as the starting point to
		// ensure we do not create an infinite loop, or add
		// the same data accidentally
		int nextRoutinePos = (inc + 1);
		while (nextRoutinePos < numOfData) {
			std::string shorthandDataItem =
					shorthandData[nextRoutinePos];
			std::string sdiBase = shorthandDataItem.substr(0,8);
			std::vector<int> setCompressArr =
					getValidSetList(shorthandDataItem);
			// Assign the data to an item
			if (isValidColonHMS(sdiBase)) {
				timeToWrite.push_back(shorthandDataItem);
				inc = nextRoutinePos;
				// Check for -1 in setCompressArr, after checking
				// for valid hour/minute/seconds/colon-use above
				// to ensure we have gotten another set and not
				// the routine name (if the colonHMS and -1 check
				// does not return -1, then the item in question
				// is a routine name)
			} else if (setCompressArr[0] != -1) {
				// If we have run through this loop at least once
				// and made it here already, we need to reset the
				// set count to output the right data and ensure
				// that the next data we add to the set is added
				// to the most recent count!
				setArr = getValidSetList(modDatum);
				if ((setCompressArr[0] == setArr[0]) &&
						(setCompressArr[1] == setArr[1])) {
					// --------------------------------------------
					// Same data with different lengths of
					// completion time...compress the data
					// --------------------------------------------
					// Update the set count
					setArr[2] += setCompressArr[2];
					// Update the datum
					modDatum = (convertIntToString(setArr[0]) +
							" X " + convertIntToString(setArr[1]) +
							" X " + convertIntToString(setArr[2]));
				} else {
					setToWrite.push_back(modDatum);
					modDatum.clear();
					modDatum = shorthandDataItem;
				}
				inc = nextRoutinePos;
			} else {
				// Here? then break...end of the road!
				break;
			}
			nextRoutinePos++;
		}
		// Here? then add the last modDatum
		setToWrite.push_back(modDatum);
		// Initialize with first item in the set...will
		// always have one item here!
		setArr = getValidSetList(setToWrite[0]);
		double weightKGS = (setArr[0] / 2.2);
		shorthandCSVFile << "," << setArr[2] <<
							"," << setArr[1] <<
							"," << setArr[0] <<
							"," << weightKGS;
		bool is2ArmTimeLength = isMatchingString(routineName,
				"dumbbell");
		// Create an increment variable to iterate through
		// the timeLengths list
		int timeInc = 0;
		// Create an increment variable to iterate through
		// the setList...we have done the first item...so
		// start with the second item
		int setInc = 1;
		if ((!is2ArmTimeLength) || (isMatchingString(routineName,
					"shrug"))) {
			int sizeOfTimeToWrite = timeToWrite.size();
			while (timeInc < sizeOfTimeToWrite) {
				shorthandCSVFile << "," << timeToWrite[timeInc];
				timeInc++;
			}
			int sizeOfSetToWrite = setToWrite.size();
			while (setInc < sizeOfSetToWrite) {
				setArr = getValidSetList(setToWrite[setInc]);
				weightKGS = (setArr[0] / 2.2);
				shorthandCSVFile << std::endl << ",," << setArr[2] <<
												  "," << setArr[1] <<
												  "," << setArr[0] <<
												  "," << weightKGS;
				setInc++;
			}
		} else {
			int placeArmDescriptor = 0;
			int sizeOfTimeToWrite = timeToWrite.size();
			while (timeInc < sizeOfTimeToWrite) {
				// Get the -Right text position
				std::string::size_type handTextPos =
						timeToWrite[timeInc].find("-Right");
				if (handTextPos != std::string::npos) {
					shorthandCSVFile << "," <<
							timeToWrite[timeInc].substr(0,
									handTextPos);
					placeArmDescriptor++;
				}
				timeInc++;
			}
			shorthandCSVFile << ((placeArmDescriptor == 3) ?
					"," : ((placeArmDescriptor == 2) ? ",," :
					((placeArmDescriptor == 1) ? ",,," : ""))) <<
					"(Right Arm)";
			// Need isOutputLeft so we do not output left-hand
			// time lengths more than once
			bool isOutputLeft = false;
			if (setToWrite.size() > 1) {
				int sizeOfSetToWrite = setToWrite.size();
				while (setInc < sizeOfSetToWrite) {
					setArr = getValidSetList(setToWrite[setInc]);
					weightKGS = (setArr[0] / 2.2);
					shorthandCSVFile << std::endl << ",," << setArr[2] <<
													  "," << setArr[1] <<
													  "," << setArr[0] <<
													  "," << weightKGS;
					if (!isOutputLeft) {
						timeInc = 0;
						int sizeOfTimeToWrite = timeToWrite.size();
						while (timeInc < sizeOfTimeToWrite) {
							// Get the -Right text position
							std::string::size_type handTextPos =
									timeToWrite[timeInc].find("-Left");
							if (handTextPos != std::string::npos)
								shorthandCSVFile << "," <<
										timeToWrite[timeInc].substr(0,
												handTextPos);
							timeInc++;
						}
						shorthandCSVFile << ((placeArmDescriptor == 3) ?
								"," : ((placeArmDescriptor == 2) ? ",," :
								((placeArmDescriptor == 1) ? ",,," : ""))) <<
								"(Left Arm)";
						isOutputLeft = true;
					}
					setInc++;
				}
			} else {
				shorthandCSVFile << std::endl << ",,,,,";
				if (!isOutputLeft) {
					timeInc = 0;
					int sizeOfTimeToWrite = timeToWrite.size();
					while (timeInc < sizeOfTimeToWrite) {
						// Get the -Right text position
						std::string::size_type handTextPos =
								timeToWrite[timeInc].find("-Left");
						if (handTextPos != std::string::npos)
							shorthandCSVFile << "," <<
									timeToWrite[timeInc].substr(0,
											handTextPos);
						timeInc++;
					}
					shorthandCSVFile << ((placeArmDescriptor == 3) ?
							"," : ((placeArmDescriptor == 2) ? ",," :
							((placeArmDescriptor == 1) ? ",,," : ""))) <<
							"(Left Arm)";
					isOutputLeft = true;
				}
			}
		}
		shorthandCSVFile << std::endl;
		inc++;
	}
	
	// Close the shorthand CSV file when finished editing
	shorthandCSVFile.close();
}

void writeSimpleView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS) {
	// Write the code
	shorthandFile << "[SIMPLEVIEW: Simple View: " <<
			((!isKGS) ? "LBS]" : "KGS]") << std::endl;
	// Write the header
	if (!isKGS)
		shorthandFile << "Weight in LBS" << std::endl;
	else
		shorthandFile << "Weight in KGS" << std::endl;
	shorthandFile << "-------------------------" << std::endl;
	// Write the date and time
	shorthandFile << std::setw(4) << " " <<
			routineDateTime << std::endl;
	// Write the data to the file
	// Get a variable to hold the current routine
	std::string routineName;
	// Get the data size to iterate through the list
	int numOfData = shorthandData.size();
	int inc = 0;
	// Add LBS data
	while (inc < numOfData) {
		// Need this variable to concatenate and later modify
		// the set data
		std::vector<std::string> dataToWrite;
		std::string datum = shorthandData[inc];
		if (datum.find("[IS_ROUTINE_NAME]") != std::string::npos) {
			shorthandFile << std::setw(8) << " " << datum.substr(0,
					datum.find("[IS_ROUTINE_NAME]")) << std::endl;
			// Set the routine's name for working with its data
			routineName = shorthandData[inc];
		} else {
			// Initial for the list to get datum...further down
			// the increment variable is incremented to ensure
			// that the time lengths (which could otherwise
			// wind up as datum) do not wind up being passed by
			std::vector<int> setArr = getValidSetList(datum);
			// If the user has entered (and probably will!) the
			// items individually, we need to compress them into
			// one line item for "Simple View"
			// Init the next position as the starting point to
			// ensure we do not create an infinite loop, or add
			// the same data accidentally
			int nextRoutinePos = (inc + 1);
			while (nextRoutinePos < numOfData) {
				std::string shorthandDataItem =
						shorthandData[nextRoutinePos];
				std::string sdiBase = shorthandDataItem.substr(0,8);
				std::vector<int> setCompressArr =
						getValidSetList(shorthandDataItem);
				// Assign the data to an item
				if (isValidColonHMS(sdiBase)) {
					dataToWrite.push_back(shorthandDataItem);
					inc = nextRoutinePos;
					// Check for -1 in setCompressArr, after checking
					// for valid hour/minute/seconds/colon-use above
					// to ensure we have gotten another set and not
					// the routine name (if the colonHMS and -1 check
					// does not return -1, then the item in question
					// is a routine name)
				} else if (setCompressArr[0] != -1) {
					// If we have run through this loop at least once
					// and made it here already, we need to reset the
					// set count to output the right data and ensure
					// that the next data we add to the set is added
					// to the most recent count!
					setArr = getValidSetList(datum);
					if ((setCompressArr[0] == setArr[0]) &&
							(setCompressArr[1] == setArr[1])) {
						// --------------------------------------------
						// Same data with different lengths of
						// completion time...compress the data
						// --------------------------------------------
						// Update the set count
						setArr[2] += setCompressArr[2];
						// Update the datum
						datum = (convertIntToString(setArr[0]) +
								" X " + convertIntToString(setArr[1]) +
								" X " + convertIntToString(setArr[2]));
						inc = nextRoutinePos;
					} else {
						// Here? Then break because even if the
						// data in the next set is the same, the
						// user did not do the sets back-to-back
						// and the data is then, different data!
						break;
					}
				} else // Here? then break...end of the road!
					break;
				nextRoutinePos++;
			}
			
			// Write out the set data
			if (!isKGS) {
				shorthandFile << std::setw(12) << " " << datum;
			} else {
				int weight;
				std::string::size_type a = datum.find(" X ");
				std::istringstream(datum.substr(0, a /* a holds the weight position */)) >> weight;
				double weightKGS = (weight / 2.2);
				// Subtracting a from c to get the position c holds!
				std::string kgsData = (convertDoubleToString(
						weightKGS) + datum.substr(a, datum.size()));
				shorthandFile << std::setw(12) << " " << kgsData;
			}
			
			// Ensure Right and Left arms are properly formatted
			// for viewing
			bool is2ArmTimeLength = isMatchingString(routineName,
					"dumbbell");
			int sI = 0;
			if ((!is2ArmTimeLength) || (isMatchingString(routineName,
					"shrug"))) {
				// Align the data to the right
				shorthandFile.setf(std::ios::right);
				int sizeOfDataToWrite = dataToWrite.size();
				while (sI < sizeOfDataToWrite) {
					shorthandFile << std::setw(12) <<
							dataToWrite[sI];
					sI++;
				}
				// Quit aligning data to the right so that
				// we can left align the next routine name
				shorthandFile.unsetf(std::ios::right);
			} else {
				// Align the data to the right
				shorthandFile.setf(std::ios::right);
				int sizeOfDataToWrite = dataToWrite.size();
				while (sI < sizeOfDataToWrite) {
					shorthandFile << std::setw(24) <<
								("Right arm: " + dataToWrite[sI].
								substr(0, dataToWrite[sI].find("-Right")));
					sI++;
					shorthandFile << std::setw(24) <<
								("Left arm: " + dataToWrite[sI].substr(0,
								dataToWrite[sI].find("-Left")));
					sI++;
				}
				// Quit aligning data to the right so that
				// we can left align the next routine name
				shorthandFile.unsetf(std::ios::right);
			}
			shorthandFile << std::endl;
		}
		inc++;
	}
	shorthandFile << std::endl << std::endl << std::endl;
}

void writeLongView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS) {
	// Write the code
	shorthandFile << "[LONGVIEW: Long View: " <<
			((!isKGS) ? "LBS]" : "KGS]") << std::endl;
	// Write the header
	if (!isKGS)
		shorthandFile << "Weight in LBS" << std::endl;
	else
		shorthandFile << "Weight in KGS" << std::endl;
	shorthandFile << "-------------------------" << std::endl;
	// Write the date and time
	shorthandFile << std::setw(4) << " " <<
			routineDateTime << std::endl;
	// Write the data to the file
	// Get a variable to hold the current routine
	std::string routineName;
	// Get the data size to iterate through the list
	int numOfData = shorthandData.size();
	int inc = 0;
	// Add LBS data
	while (inc < numOfData) {
		// Need this variable to concatenate and later modify
		// the set data
		std::vector<std::string> dataToWrite;
		std::string datum = shorthandData[inc];
		if (datum.find("[IS_ROUTINE_NAME]") != std::string::npos) {
			shorthandFile << std::setw(8) << " " << datum.substr(0,
					datum.find("[IS_ROUTINE_NAME]")) << std::endl;
			// Set the routine's name for working with its data
			routineName = shorthandData[inc];
		} else {
			// Initial for the list to get datum...further down
			// the increment variable is incremented to ensure
			// that the time lengths (which could otherwise
			// wind up as datum) do not wind up being passed by
			std::vector<int> setArr = getValidSetList(datum);
			// If the user has entered (and probably will!) the
			// items individually, we need to compress them into
			// one line item for "Simple View"
			// Init the next position as the starting point to
			// ensure we do not create an infinite loop, or add
			// the same data accidentally
			int nextRoutinePos = (inc + 1);
			while (nextRoutinePos < numOfData) {
				std::string shorthandDataItem =
						shorthandData[nextRoutinePos];
				std::string sdiBase = shorthandDataItem.substr(0,8);
				std::vector<int> setCompressArr =
						getValidSetList(shorthandDataItem);
				// Assign the data to an item
				if (isValidColonHMS(sdiBase)) {
					dataToWrite.push_back(shorthandDataItem);
					inc = nextRoutinePos;
					// Check for -1 in setCompressArr, after checking
					// for valid hour/minute/seconds/colon-use above
					// to ensure we have gotten another set and not
					// the routine name (if the colonHMS and -1 check
					// does not return -1, then the item in question
					// is a routine name)
				} else if (setCompressArr[0] != -1) {
					// If we have run through this loop at least once
					// and made it here already, we need to reset the
					// set count to output the right data and ensure
					// that the next data we add to the set is added
					// to the most recent count!
					setArr = getValidSetList(datum);
					if ((setCompressArr[0] == setArr[0]) &&
							(setCompressArr[1] == setArr[1])) {
						// --------------------------------------------
						// Same data with different lengths of
						// completion time...compress the data
						// --------------------------------------------
						// Update the set count
						setArr[2] += setCompressArr[2];
						// Update the datum
						datum = (convertIntToString(setArr[0]) +
								" X " + convertIntToString(setArr[1]) +
								" X " + convertIntToString(setArr[2]));
						inc = nextRoutinePos;
					} else {
						// Here? Then break because even if the
						// data in the next set is the same, the
						// user did not do the sets back-to-back
						// and the data is then, different data!
						break;
					}
				} else // Here? then break...end of the road!
					break;
				nextRoutinePos++;
			}
			
			// Write out the set data
			if (!isKGS) {
				// For good measure, reset the setArr to the datum
				// we are writing for the user
				setArr = getValidSetList(datum);
				// Output the place-holder
				shorthandFile << std::setw(12) << " ";
				// Output the weight in LBS
				shorthandFile << convertIntToString(setArr[0]);
				// Now, increment until the set count is reached
				// so that we can output the REPS X REPS portion
				for (int repsInc = 0; repsInc < setArr[2]; repsInc++) {
					shorthandFile << " X " << setArr[1];
				}
			} else {
				// For good measure, reset the setArr to the datum
				// we are writing for the user
				setArr = getValidSetList(datum);
				double weightKGS = (setArr[0] / 2.2);
				// Subtracting a from c to get the position c holds!
				std::string kgsData = convertDoubleToString(weightKGS);
				// Output the place-holder
				shorthandFile << std::setw(12) << " ";
				// Output the weight in KGS
				shorthandFile << kgsData;
				// Now, increment until the set count is reached
				// so that we can output the "X REPS X REPS" portion
				for (int repsInc = 0; repsInc < setArr[2]; repsInc++) {
					shorthandFile << " X " << setArr[1];
				}
			}
			
			// Ensure Right and Left arms are properly formatted
			// for viewing
			bool is2ArmTimeLength = isMatchingString(routineName,
					"dumbbell");
			int sI = 0;
			if ((!is2ArmTimeLength) || (isMatchingString(routineName,
					"shrug"))) {
				// Align the data to the right
				shorthandFile.setf(std::ios::right);
				int sizeOfDataToWrite = dataToWrite.size();
				while (sI < sizeOfDataToWrite) {
					shorthandFile << std::setw(12) <<
							dataToWrite[sI];
					sI++;
				}
				// Quit aligning data to the right so that
				// we can left align the next routine name
				shorthandFile.unsetf(std::ios::right);
			} else {
				// Align the data to the right
				shorthandFile.setf(std::ios::right);
				int sizeOfDataToWrite = dataToWrite.size();
				while (sI < sizeOfDataToWrite) {
					shorthandFile << std::setw(24) <<
								("Right arm: " + dataToWrite[sI].
								substr(0, dataToWrite[sI].find("-Right")));
					sI++;
					shorthandFile << std::setw(24) <<
								("Left arm: " + dataToWrite[sI].substr(0,
								dataToWrite[sI].find("-Left")));
					sI++;
				}
				// Quit aligning data to the right so that
				// we can left align the next routine name
				shorthandFile.unsetf(std::ios::right);
			}
			shorthandFile << std::endl;
		}
		inc++;
	}
	shorthandFile << std::endl << std::endl << std::endl;
}

void writeListColumnView(std::ofstream &shorthandFile,
		std::string fullFilePath,
		std::string routineDateTime,
		std::vector<std::string> shorthandData,
		bool isKGS) {
	// Write the code
	shorthandFile << "[LISTCOLUMNVIEW: List Column View: " <<
			((!isKGS) ? "LBS]" : "KGS]") << std::endl;
	// Write the header
	if (!isKGS)
		shorthandFile << "Weight in LBS" << std::endl;
	else
		shorthandFile << "Weight in KGS" << std::endl;
	shorthandFile << "-------------------------" << std::endl;
	// Write the date and time
	shorthandFile << std::setw(4) << " " <<
			routineDateTime << std::endl;
	// Write the data to the file
	// Get a variable to hold the current routine
	std::string routineName;
	// Get the data size to iterate through the list
	int numOfData = shorthandData.size();
	int inc = 0;
	// Add LBS data
	while (inc < numOfData) {
		// Need this variable to concatenate and later modify
		// the set data
		std::vector<std::string> dataToWrite;
		std::string datum = shorthandData[inc];
		if (datum.find("[IS_ROUTINE_NAME]") != std::string::npos) {
			shorthandFile << std::setw(8) << " " << datum.substr(0,
					datum.find("[IS_ROUTINE_NAME]")) << std::endl;
			// Set the routine's name for working with its data
			routineName = shorthandData[inc];
		} else {
			// Initial for the list to get datum...further down
			// the increment variable is incremented to ensure
			// that the time lengths (which could otherwise
			// wind up as datum) do not wind up being passed by
			std::vector<int> setArr = getValidSetList(datum);
			// If the user has entered (and probably will!) the
			// items individually, we need to compress them into
			// one line item for "Simple View"
			// Init the next position as the starting point to
			// ensure we do not create an infinite loop, or add
			// the same data accidentally
			int nextRoutinePos = (inc + 1);
			while (nextRoutinePos < numOfData) {
				std::string shorthandDataItem =
						shorthandData[nextRoutinePos];
				std::string sdiBase = shorthandDataItem.substr(0,8);
				std::vector<int> setCompressArr =
						getValidSetList(shorthandDataItem);
				// Assign the data to an item
				if (isValidColonHMS(sdiBase)) {
					dataToWrite.push_back(shorthandDataItem);
					inc = nextRoutinePos;
					// Check for -1 in setCompressArr, after checking
					// for valid hour/minute/seconds/colon-use above
					// to ensure we have gotten another set and not
					// the routine name (if the colonHMS and -1 check
					// does not return -1, then the item in question
					// is a routine name)
				} else if (setCompressArr[0] != -1) {
					// If we have run through this loop at least once
					// and made it here already, we need to reset the
					// set count to output the right data and ensure
					// that the next data we add to the set is added
					// to the most recent count!
					setArr = getValidSetList(datum);
					if ((setCompressArr[0] == setArr[0]) &&
							(setCompressArr[1] == setArr[1])) {
						// --------------------------------------------
						// Same data with different lengths of
						// completion time...compress the data
						// --------------------------------------------
						// Update the set count
						setArr[2] += setCompressArr[2];
						// Update the datum
						datum = (convertIntToString(setArr[0]) +
								" X " + convertIntToString(setArr[1]) +
								" X " + convertIntToString(setArr[2]));
						inc = nextRoutinePos;
					} else {
						// Here? Then break because even if the
						// data in the next set is the same, the
						// user did not do the sets back-to-back
						// and the data is then, different data!
						break;
					}
				} else // Here? then break...end of the road!
					break;
				nextRoutinePos++;
			}
			
			// Write out the set data
			if (!isKGS) {
				// For good measure, reset the setArr to the datum
				// we are writing for the user
				setArr = getValidSetList(datum);
				std::string weight = convertIntToString(setArr[0]);
				// Now, increment until the set count is reached
				// so that we can output the REPS X REPS portion
				for (int repsInc = 0; repsInc < setArr[2]; repsInc++) {
					// Output the place-holder
					shorthandFile << std::setw(12) << " ";
					// Output the weight in LBS
					shorthandFile << weight;
					// Output the reps portion
					shorthandFile << " X " << setArr[1];
					// Output the single time
					_specTimeOutput(shorthandFile, routineName,
							dataToWrite, repsInc);
					// Output the new line
					shorthandFile << std::endl;
				}
			} else {
				// For good measure, reset the setArr to the datum
				// we are writing for the user
				setArr = getValidSetList(datum);
				double weightKGS = (setArr[0] / 2.2);
				// Subtracting a from c to get the position c holds!
				std::string kgsData = convertDoubleToString(weightKGS);
				// Now, increment until the set count is reached
				// so that we can output the "X REPS X REPS" portion
				for (int repsInc = 0; repsInc < setArr[2]; repsInc++) {
					// Output the place-holder
					shorthandFile << std::setw(12) << " ";
					// Output the weight in KGS
					shorthandFile << kgsData;
					// Output the reps portion
					shorthandFile << " X " << setArr[1];
					// Output the single time
					_specTimeOutput(shorthandFile, routineName,
							dataToWrite, repsInc);
					// Output the new line
					shorthandFile << std::endl;
				}
			}
		}
		inc++;
	}
	shorthandFile << std::endl << std::endl << std::endl;
}

void _specTimeOutput(std::ofstream &shorthandFile,
		std::string routineName,
		std::vector<std::string> dataToWrite,
		int timePos) {
	// Ensure Right and Left arms are properly formatted
	// for viewing
	bool is2ArmTimeLength = isMatchingString(routineName,
			"dumbbell");
	if ((!is2ArmTimeLength) || (isMatchingString(routineName,
			"shrug"))) {
		// Align the data to the right
		shorthandFile.setf(std::ios::right);
		shorthandFile << std::setw(12) <<
				dataToWrite[timePos];
		// Quit aligning data to the right so that
		// we can left align the next routine name
		shorthandFile.unsetf(std::ios::right);
	} else {
		timePos *= 2;
		// Align the data to the right
		shorthandFile.setf(std::ios::right);
		shorthandFile << std::setw(24) <<
					("Right arm: " + dataToWrite[timePos].
					substr(0, dataToWrite[timePos].find("-Right")));
		timePos++;
		shorthandFile << std::setw(24) <<
					("Left arm: " + dataToWrite[timePos].substr(0,
					dataToWrite[timePos].find("-Left")));
		// Quit aligning data to the right so that
		// we can left align the next routine name
		shorthandFile.unsetf(std::ios::right);
	}
}

bool isEnd(std::string endCommand) {
	// Declare the variable
	bool isEnded = false;
	
	// Check for end input
	if (isMatchingString(endCommand, "[end;]") ||
		isMatchingString(endCommand, "end"))
		isEnded = true;
	
	// Return the value
	return isEnded;
}

std::vector<std::string> getShorthandData(void) {
	// NOTE: The help feature functionality implemented
	// to prevent user(s) from accidentally mistyping
	// a routine and for ease of use!
	// Declare the help arguments
	//std::string helpPath = "C:\\Env\\Dev\\PUI\\app\\PUI\\categories\\sporting\\bodybuilding\\RoutineList.txt";
	std::string helpPath = "auxfiles\\Bodybuilding-Routine-List.txt";
	std::string helpSwitches = " <shorthand> /N";
	// After creating the options window arguments, create
	// an item vector to store the options list in, and to
	// allow the user to select the option from the list
	std::ifstream *helpData = readFile(helpPath.c_str());
	std::vector<std::string> options;
	// User input will be 1 - #, therefore, no zero-based
	// options list can be accepted...add a null item
	// to fix this
	options.push_back("");
	std::string optionItem;
	while(getline(*helpData, optionItem)) {
		options.push_back(optionItem);
	}
	// Clear some memory
	optionItem.clear();
	
	// Declare the input string
	std::vector<std::string> shorthandData;
	
	bool isQuit = false;
	// Get the input until the user says end or quit
	while (true) {
		// If the routine info loop has already ran,
		// we need to check if isQuit has been set
		if (isQuit)
			break;
		// Declare the input holder
		std::string inputRoutine;
		std::string routineName;
		std::cout << "\nOption [Set Count]:\n> ";
		int InputControl[2];
		InputControl[0] = GetX();
		InputControl[1] = GetY();
		getline(std::cin, inputRoutine);
		// Check for quit, end, and help in routine inquiry
		if (isClose(inputRoutine)) {
			isQuit = true;
			break;
		} else if (isEnd(inputRoutine)) {
			// No more checks necessary for isEnd
			break;
		} else if (isMatchingString(
				inputRoutine, "help")) {
			// Clear the screen and output the intro again
			ClearScreen();
			getIntroduction("shorthandroutine");
			// Display the help menu
			help((helpPath + helpSwitches).c_str());
			continue;
		}
		// Validate user input and get the set count
		int setCount = validateRoutineSetCount(inputRoutine);
		if (setCount == -1) {
			std::cout << "^ Invalid input!\n";
			continue;
		}
		if (inputRoutine.find(" [") != std::string::npos)
			routineName = inputRoutine.substr(0, inputRoutine.
					find(" ["));
		else
			routineName = inputRoutine.substr(0, inputRoutine.
					find(']'));
		int selectedOption;
		std::istringstream(routineName) >> selectedOption;
		// Because of the NULL zero-place in the options vector,
		// less than size of the list is required
		int sizeOfOptions = options.size();
		if ((selectedOption < sizeOfOptions) &&
				(selectedOption > 0)) {
			std::string optionFile = options[selectedOption];
			std::string optionDisplay = options[selectedOption];
			if (optionFile.find("[DEGREE;]") != std::string::npos) {
				optionFile.replace(optionFile.find("[DEGREE;]"), 9, "\xB0");
				optionDisplay.replace(optionDisplay.find("[DEGREE;]"), 9, "\370");
			}
			routineName = (optionFile + "[IS_ROUTINE_NAME]");
			optionDisplay += (", " + convertIntToString(setCount) + " sets");
			// Get the position of the next input point
			int InputControlNext[2];
			InputControlNext[0] = 0;
			// Increment the row here right now, so that
			// when we pass the row to function populateData,
			// the correct row will be populated and the next
			// row will receive the position at position 0,row
			InputControlNext[1] = ++InputControl[1];
			populateData(InputControl[0], InputControl[1], 75,
						optionDisplay, InputControlNext);
		} else {
			std::cout << "^ Invalid input!\n";
			continue;
		}
		shorthandData.push_back(routineName);
		
		std::string inputSet;
		std::vector<int> routineSet;
		std::string inputLength;
		std::vector<std::string> setLength;
		// Declare the set input counter
		int setInputCounter = 0;
		// Get the data for the routine while set counter
		// less than or equal to the set count
		while (setInputCounter < setCount) {
			std::cout << "  > ";
			getline(std::cin, inputSet);
			if ((isClose(inputSet)) || isQuit) {
				isQuit = true;
				break;
			}
			routineSet = getValidSetList(inputSet);
			if (routineSet[0] == -1) {
				std::cout << "    ^ Invalid input!\n";
				continue;
			} else {
				if (routineSet.size() == 3) {
					if ((routineSet[2] + setInputCounter) > setCount)
						routineSet[2] = (setCount - setInputCounter);
					setInputCounter += routineSet[2];
					// Dumbbell Counter
					int dumbbellCounter = 0;
					// Set Counter
					int nI = 0;
					while (nI < routineSet[2]) {
						if ((isMatchingString(routineName,
								"dumbbell")) &&
								(!isMatchingString(routineName,
										"shrug"))) {
							if (dumbbellCounter == 0)
								std::cout << "Right arm  > ";
							else if (dumbbellCounter == 1)
								std::cout << "Left arm   > ";
						} else
							std::cout << "    > ";
						getline(std::cin, inputLength);
						if (isClose(inputSet)) {
							isQuit = true;
							break;
						} else if (!isValidColonHMS(inputLength)) {
							std::cout << "        ^ Invalid input!\n";
							continue;
						} else {
							if (isMatchingString(routineName,
									"dumbbell") &&
									(!isMatchingString(routineName,
											"shrug"))) {
								setLength.push_back(inputLength);
								if (dumbbellCounter == 0) {
									dumbbellCounter++;
									continue;
								} else
									dumbbellCounter = 0;
							} else
								setLength.push_back(inputLength);
						}
						nI++;
					}
				} else {
					std::cout << "  <- Invalid input!";
					continue;
				}
			}
			if (!isQuit) {
				std::string data = (convertIntToString(routineSet[0]) +
							" X " + convertIntToString(routineSet[1]) +
							" X " + convertIntToString(routineSet[2]));
				shorthandData.push_back(data);
				int sI = 0;
				bool is2ArmTimeLength = isMatchingString(routineName,
						"dumbbell");
				int sizeOfSetLength = setLength.size();
				while (sI < sizeOfSetLength) {
					if ((!is2ArmTimeLength) || (isMatchingString(routineName,
							"shrug")))
						shorthandData.push_back(setLength[sI]);
					else {
						shorthandData.push_back(setLength[sI] + "-Right");
						shorthandData.push_back(setLength[sI+=1] + "-Left");
					}
					sI++;
				}
				// Clear the set length data so the data is not
				// added to the next set item constantly!!!
				setLength.clear();
			}
		}
	}
	
	if (isQuit) {
		if (!shorthandData.size())
			shorthandData.push_back("-1");
		else
			shorthandData[0] = "-1";
	}
	
	// Return the value
	return shorthandData;
}

std::string removeSpaces(std::string input) {
	// Erase spaces
	input.erase(std::remove(input.begin(),
			input.end(), ' '), input.end());
	
	// Return the value
	return input;
}

bool hasOnlyValidCharsIntXxSpace(std::string &checkMe) {
	bool isInvalid = false;
	// First time through, we want a digit
	bool isInitDigit = false;
	// Second time through, we want a space
	// (xX does not matter)
	bool isInitSpace = false;
	// Lastly, and the point of this, we want
	// the user to have entered a number, a
	// space, and a number (this prevents the
	// user from only entering the weight,
	// neglecting the repetitions)
	bool isSecondNumber = false;
	int sizeOfCheckMe = checkMe.size();
	for (int i = 0; i < sizeOfCheckMe; i++) {
		if ((checkMe[i] <= '9') && (checkMe[i] >= '0'))	{
			isInitDigit = true;
			if (isInitDigit && isInitSpace)
				isSecondNumber = true;
			continue;
		} else if ((checkMe[i] == 'x') || (checkMe[i] == 'X'))
			continue;
		else if (checkMe[i] == ' ') {
			if (isInitDigit)
				isInitSpace = true;
			continue;
		} else {
			isInvalid = true;
			break;
		}
	}
	
	// Check to ensure two numbers have been entered
	if (!(isInitDigit && isSecondNumber))
		isInvalid = true;
	
	// Return the value
	return isInvalid;
}

bool isValidColonHMS(std::string &checkMe) {
	bool isValid = true;
	// When finished, we should have 6 digits, 2 colons
	int digCount = 0;
	int colonCount = 0;
	int sizeOfCheckMe = checkMe.size();
	for (int i = 0; i < sizeOfCheckMe; i++) {
		if ((checkMe[i] <= '9') && (checkMe[i] >= '0')) {
			digCount++;
			if ((colonCount == 0) && (digCount == 3)) {
				isValid = false;
				break;
			} else if ((colonCount == 1) && (digCount == 5)) {
				isValid = false;
				break;
			} else if ((colonCount == 2) && (digCount == 7)) {
				isValid = false;
				break;
			}
			continue;
		} else if (checkMe[i] == ':') {
			colonCount++;
		} else {
			isValid = false;
			break;
		}
	}

	// To help prevent a stray colon at the end
	if ((colonCount != 2) || (digCount != 6))
		isValid = false;
	
	// Return the value
	return isValid;
}

std::vector<int> getValidSetList(std::string inputSet) {
	// Returns either ##X## or ##X##X##
	// Declare the validSet vector/array
	std::vector<int> validSet;
	// Get the size of the inputSet string
	int inputSetSize = inputSet.size();
	
	// Declare the isInvalid boolean and validate the input
	bool isInvalid = hasOnlyValidCharsIntXxSpace(inputSet);
	if (!isInvalid) {
		// Need to know the size of the input
		int weightInt = 0;
		int repsInt = 0;
		int setsInt = 0;
		// Need three booleans to know when to do what
		// Weight is always first, so initially it is true
		bool goForWeight = true;
		bool goForReps = false;
		bool goForSets = false;
		
		const char *seps = "x X";
		const char *digs = "0123456789";
		std::string::size_type b = inputSet.find_first_of(digs, 0, 10);
		std::string::size_type n = inputSet.find_first_of(seps, b, 3);
		// Loop through the array to find digits
		for (;;) {
			if (n == std::string::npos)
				n == inputSetSize;
			// If b is not found, no data, break
			if (b == std::string::npos)
				break;
			if (goForWeight) {
				std::istringstream(inputSet.substr(b,n)) >> weightInt;
				goForWeight = false;
				goForReps = true;
			} else if (goForReps) {
				std::istringstream(inputSet.substr(b,n)) >> repsInt;
				goForReps = false;
				goForSets = true;
			} else if (goForSets) {
				// Need the goForSets check because the user
				// may accidentally add more data, and we do
				// not want the right data removed! So we do
				// the check and set the variable to false
				std::istringstream(inputSet.substr(b,n)) >> setsInt;
				goForSets = false;
			}
			b = inputSet.find_first_of(digs,n);
			n = inputSet.find_first_of(seps,b);
		} // End Loop
		// Because b may exit the loop when sets are not
		// present we need to check for a zero-value
		if (setsInt == 0)
			setsInt = 1;
		validSet.push_back(weightInt);
		validSet.push_back(repsInt);
		validSet.push_back(setsInt);
	} else {
		validSet.push_back(-1);
		validSet.push_back(-1);
		validSet.push_back(-1);
	}
	
	// Return the value
	return validSet;
}

int validateRoutineSetCount(std::string routine) {
	// Declare the count variable
	int count = 0;
	
	// Get the positions of the "[" and "]"
	std::string::size_type beginPos = routine.rfind('[');
	std::string::size_type endPos = routine.rfind(']');
	
	if ((beginPos == std::string::npos) ||
			(endPos == std::string::npos))
		count = -1;
	
	if (count != -1) {
		std::istringstream(routine.substr(beginPos + 1,
				endPos - 1)) >> count;
		if (!(count > 0))
			count = -1;
	}
	
	// Return the value
	return count;
}

void dumpShorthandRoutineData(std::string filePath,
		struct ShorthandRoutineStruct shorthandRoutineData) {
	// Dump data to file in __dump__ directory
	std::ofstream dataDumpFile;
	// Open the file
	dataDumpFile.open(filePath.c_str(), std::ios::binary);
	
	// Output the data to the file
	dataDumpFile << "[TXT_FILE_NAME]" << std::endl <<
			shorthandRoutineData.textFileName << std::endl <<
			std::endl;
	dataDumpFile << "[CSV_FILE_NAME]" << std::endl <<
			shorthandRoutineData.csvFileName << std::endl <<
			std::endl;
	dataDumpFile << "[TXT_FILE_PATH]" << std::endl <<
			shorthandRoutineData.fullFilePath << std::endl <<
			std::endl;
	dataDumpFile << "[CSV_FILE_PATH]" << std::endl <<
			shorthandRoutineData.fullCSVFilePath << std::endl <<
			std::endl;
	dataDumpFile << "[WEEKLY_FOLDER_NAME]" << std::endl <<
			shorthandRoutineData.weeklyFolderName << std::endl <<
			std::endl;
	
	dataDumpFile << "[SHORTHAND_DATA]" << std::endl;
	int sizeOfData = shorthandRoutineData.shorthandData.size();
	for (int i = 0; i < sizeOfData; i++)
		dataDumpFile << shorthandRoutineData.
				shorthandData[i] << std::endl;
	
	// Close the file
	dataDumpFile.close();
}