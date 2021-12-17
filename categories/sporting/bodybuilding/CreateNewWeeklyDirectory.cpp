// Function Prototypes
// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
// Regular includes
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\..\..\include\conf.h"
#include "..\..\..\include\CreateNewWeeklyDirectory.h"
#include "..\..\..\include\DateTime.h"
#include "..\..\..\include\FileIO.h"
#include "..\..\..\include\FileSystem.h"
#include "..\..\..\include\Interface.h"
#include "..\..\..\include\ShellControls.h"
#include "..\..\..\include\WindowsConsole.h"

using namespace std;

using namespace PUI;

// Main function
int CreateNewWeeklyDirectory() {
	// Clear the screen
	ClearScreen();
	
	// Get the user's bodybuilding directory for storing weekly data
	std::string bodybuildingDirectory =
			(config.getValue("Module.Sports.Bodybuilding.Directory") +
			"Weekly/");
	
	// Get Date/Time
	struct tm currentDateTime = GetCurrentDateTM();
	struct DayAndWeekInfo currentDayWeek;
	currentDayWeek = getDayAndWeekInfo(currentDateTime);
	
	// Create the new directory's name
	std::string dirName = ("Week " +
			currentDayWeek.dayDTI.pui_gdt_str_year + "-" +
			currentDayWeek.dayDTI.pui_gdt_str_week);
	std::string dirInfoFileName = (dirName + ".info");
	
	// Check if the directory by current year exists:
	//	- If it does not, then create it
	//	- Update the bodybuildingDirectory to reflect the change
	std::string currentWeeklyYearDirectory = (bodybuildingDirectory +
			currentDayWeek.dayDTI.pui_gdt_str_year + "/");
	if (!directoryExists(currentWeeklyYearDirectory.c_str())) {
		CreateDirectory(currentWeeklyYearDirectory.c_str(), NULL);
	}
	
	// 1. Get the file/folder path
	// 2. Ensure the correct Windows path separator is used: "\\" for Windows
	std::string newWeeklyDirectory = (currentWeeklyYearDirectory +
			dirName + "/");
		size_t start_pos = 0;
	std::string searchTerm = "/";
	std::string replaceTerm = "\\";
	while((start_pos = newWeeklyDirectory.find(searchTerm, start_pos)) != std::string::npos) {
		newWeeklyDirectory.replace(start_pos, searchTerm.length(), replaceTerm);
		start_pos += replaceTerm.length();
	}
	
	// Get the Introduction
	getIntroduction("newweeklyroutinedirectory");
	std::cout << std::endl << std::endl << "Working..." << std::endl << std::endl;
	waitOneMoment();
	
	// Check if the Application has been ran for
	// the current week
	if (fileExists((newWeeklyDirectory + dirInfoFileName).c_str())) {
		std::cout << "Sorry, the current week's directory structure has already been\ncreated. Exiting the program.\n\n";
		
		// Pause the screen
		Pause(true);
		
		// Return the value
		return 0;
	}

	// Create the new directory
	std::cout << "\nCreating the selected week's directory...";
	CreateDirectory(newWeeklyDirectory.c_str(), NULL);
	
	// Create the new directory's Shorthand directory for the Create a New Shorthand Routine Application
	std::cout << "\nCreating the \"Shorthand\" directory...\n";
	CreateDirectory((newWeeklyDirectory + "Shorthand").c_str(), NULL);
	
	// Output new information for user's sake
	std::cout << "New directory name: " << dirName << std::endl;
	std::cout << "New directory path: " << newWeeklyDirectory << std::endl;
	
	// Alert that we are copying data for weekly files
	std::cout << "Copying the standard weekly files..." << std::endl;
	std::cout << "    Warning! This may take up to a minute!" << std::endl;
	
	////////////////////////////////////////////////////////////////////////////////
	// BEGIN COPYING FILES
	////////////////////////////////////////////////////////////////////////////////
	// $ bin.exe WeeklyRoutineDataProcessingFileList.txt BodybuildingDataProcessingSheet.bin -encode -am
	// $ protect.exe BodybuildingDataProcessingSheet.bin BodybuildingDataProcessingSheet 3
	
	// -----------------------------------------------------------------------------
	// FIRST: Make the PUI temporary folder
	// -----------------------------------------------------------------------------
	std::string temporaryFolder;
	DWORD dwRetVal = 0;
    TCHAR lpTempPathBuffer[MAX_PATH];
	//  Gets the temp path env string (no guarantee it's a valid path).
    dwRetVal = GetTempPath(MAX_PATH,          // length of the buffer
                           lpTempPathBuffer); // buffer for path 
    if (dwRetVal > MAX_PATH || (dwRetVal == 0))
		std::cout << std::endl << std::endl << "oh god, an error!" << std::endl << std::endl;
	else
		temporaryFolder = lpTempPathBuffer;
	// Now that we have a handle on the user's temporary directory,
	// create the temporary PUI processing folder and assign that
	// new folder as our working temporaryFolder.
	CreateDirectory((temporaryFolder + "PUI").c_str(), NULL);
	temporaryFolder += "PUI/";
	// -----------------------------------------------------------------------------
	// SECOND: Begin decoding weekly files and gather the list of files
	// -----------------------------------------------------------------------------
	std::string datasheetPath = config.getValue("DATAPROCF.Module.Sports.Bodybuilding.DataSheets");
	std::vector<std::vector<std::string> > binaryDatasheet = decodeBinaryFile(datasheetPath.c_str());
	std::vector<std::string> filesToCopy = getEncodedFilesListFromMemory(binaryDatasheet);
	int sizeOfFilesToCopy = filesToCopy.size();
	// -----------------------------------------------------------------------------
	// THIRD: Create all temporary directories that will house the data needed for
	//		  the MS Office files: *.xlsx and *.docx; place these new directories
	//		  inside the temporary folder used for PUI operations.
	// -----------------------------------------------------------------------------
	int directoryCreationIterator = 0;
	while (directoryCreationIterator < sizeOfFilesToCopy) {
		std::string::size_type n = filesToCopy[directoryCreationIterator].find("/");
		while (n != std::string::npos) {
			CreateDirectory((temporaryFolder +
					filesToCopy[directoryCreationIterator].
								substr(0, n)).c_str(), NULL);
			n = filesToCopy[directoryCreationIterator].find("/", n+1);
		}
		directoryCreationIterator++;
	}
	for (int i = 0; i < sizeOfFilesToCopy; i++)
	// -----------------------------------------------------------------------------
	// FOURTH: Replace variables in any files with variable data accordingly
	// FIFTH: Write all new files and variable data
	// -----------------------------------------------------------------------------
		writeDecodedBinaryWeeklyRoutinesFilesFromMemory(filesToCopy, binaryDatasheet,
				(temporaryFolder + filesToCopy[i]), filesToCopy[i], currentDayWeek);
		
	// -----------------------------------------------------------------------------
	// SIXTH: Zip all files "inside" the top folder "Charts", "Notes", "Routines",
	//		  and "Warnings" into .xlsx and .docx (x3) file extensions
	// -----------------------------------------------------------------------------
	std::string commonCommands = ("a -sdel \"" + temporaryFolder);
	CreateZipFolder(commonCommands + "Charts.xlsx\" \"" + temporaryFolder + "Charts/*\"");
	CreateZipFolder(commonCommands + "Notes.docx\" \"" + temporaryFolder + "Notes/*\"");
	CreateZipFolder(commonCommands + "Routines.docx\" \"" + temporaryFolder + "Routines/*\"");
	CreateZipFolder(commonCommands + "Warnings.docx\" \"" + temporaryFolder + "Warnings/*\"");
	// Due to a problem regarding FileIO manipulation, waitOneMoment() is required.
	// The FileIO problem consists of either a delay in the file system finding
	// and/or recognizing the new files created using CreateZipFolder and 7-zip, or
	// in the file system's ability to move those files.
	waitOneMoment();
	// Push items to list for printing in information file and moving those files
	// to the new weekly routine directory.
	std::vector<std::string> weeklyRoutineFilesList;
	weeklyRoutineFilesList.push_back("Charts.xlsx");
	weeklyRoutineFilesList.push_back("Notes.docx");
	weeklyRoutineFilesList.push_back("Routines.docx");
	weeklyRoutineFilesList.push_back("Warnings.docx");
	// Iterate through the items in weeklyRoutineFilesList to move those files to
	// the new weekly routine directory.
	int iterationRoutineList = 0;
	while (iterationRoutineList < 4) {
		MoveFile((temporaryFolder + weeklyRoutineFilesList[iterationRoutineList]).c_str(),
				(newWeeklyDirectory + weeklyRoutineFilesList[iterationRoutineList]).c_str());
		iterationRoutineList++;
	}
	// -----------------------------------------------------------------------------
	// SEVENTH: Delete all parent folders used in the SIXTH step; note that all
	//			files compressed were deleted in the SIXTH step by 7-zip's 7z.exe.
	//			Delete all temporary files created in the SIXTH step.
	// -----------------------------------------------------------------------------
	RemoveDirectory((temporaryFolder + "Charts").c_str());
	RemoveDirectory((temporaryFolder + "Notes").c_str());
	RemoveDirectory((temporaryFolder + "Routines").c_str());
	RemoveDirectory((temporaryFolder + "Warnings").c_str());
	
	////////////////////////////////////////////////////////////////////////////////
	// END COPYING FILES
	////////////////////////////////////////////////////////////////////////////////
	
	// Create the weekly directory info file
	std::cout << "Creating the new week's directory information file..." << std::endl;
	createDirectoryInfoFile(newWeeklyDirectory, dirInfoFileName,
			currentDayWeek, weeklyRoutineFilesList);
	
	// Backup last data by directory, not the files therein
	// backupLastDirectory(currentDayWeek, bodybuildingDirectory);
	
	std::cout << "Opening the directory path for viewing..." << std::endl << std::endl << std::endl;
	ShellExecuteA(NULL, "open", "explorer.exe", newWeeklyDirectory.c_str(), NULL, SW_SHOWDEFAULT);
	//ShellExecuteA(NULL, "open", "explorer.exe", "C:\\Users\\PUI-0103\\Documents\\Sports\\Bodybuilding\\Routine Information\\Weekly\\2020\\Week 2020-10\\", NULL, SW_SHOWDEFAULT);

	// Pause the screen
	Pause(true);
	
	// Return the value
	return 0;
}

std::string GetBodybuildingWeekDateCodeInfo(
		struct PUIVisibleDTS beginDayOfWeek,
		struct PUIVisibleDTS endDayOfWeek) {
	std::string begin_mday = beginDayOfWeek.pui_gdt_str_mday;
	std::string begin_mon = beginDayOfWeek.pui_gdt_str_mon;
	std::string begin_int_mon = beginDayOfWeek.pui_gdt_str_int_mon;
	std::string begin_year = beginDayOfWeek.pui_gdt_str_year;
	std::string begin_wday = beginDayOfWeek.pui_gdt_str_wday;
	std::string begin_int_wday = beginDayOfWeek.pui_gdt_str_int_wday;
	std::string begin_yday = beginDayOfWeek.pui_gdt_str_yday;
	std::string begin_week = beginDayOfWeek.pui_gdt_str_week;
	
	std::string end_mday = endDayOfWeek.pui_gdt_str_mday;
	std::string end_mon = endDayOfWeek.pui_gdt_str_mon;
	std::string end_int_mon = endDayOfWeek.pui_gdt_str_int_mon;
	std::string end_year = endDayOfWeek.pui_gdt_str_year;
	std::string end_wday = endDayOfWeek.pui_gdt_str_wday;
	std::string end_int_wday = endDayOfWeek.pui_gdt_str_int_wday;
	std::string end_yday = endDayOfWeek.pui_gdt_str_yday;
	std::string end_week = endDayOfWeek.pui_gdt_str_week;
	
	std::string WBTCode = getWBTCode(begin_int_wday, begin_mday,
			begin_int_mon, begin_year, end_int_wday, end_mday,
			end_int_mon, end_year);
			
	std::string BBWeekDateData = "Date (Code):\n";
	BBWeekDateData += "---------------\n";
	BBWeekDateData += ("> " + begin_wday + ", " + begin_mday + " " +
			begin_mon + " " + begin_year + " (" + WBTCode.substr(0,10) +
			") -\n");
	BBWeekDateData += ("\t" + end_wday + ", " + end_mday + " " +
			end_mon + " " + end_year + " (" + WBTCode.substr(10,20) +
			")\n");
	BBWeekDateData += ("> WBT* Code: " + WBTCode);
	
	// > Thursday, 15 January 2015 (0415012015) -
	//		Wednesday, 21 January 2015 (0321012015)
	// > WBT* Code: 04150120150321012015
	
	// Return the value
	return BBWeekDateData;
}

void createDirectoryInfoFile(std::string newWeeklyDirectory,
		std::string dirInfoFileName,
		struct DayAndWeekInfo currentDayWeek,
		std::vector<std::string> filesCopied) {
	// Open the file stream to write data to the new file
	std::ofstream dirInfoFile;
	
	// Open the info file for editing
	dirInfoFile.open((newWeeklyDirectory + dirInfoFileName).c_str());
	
	// Populate the info file with data
	dirInfoFile << "*****************************" << std::endl;
	dirInfoFile << "* " << currentDayWeek.dayDTI.pui_gdt_str_year <<
			": Week " << currentDayWeek.dayDTI.pui_gdt_str_week <<
			" Information *" << std::endl;
	dirInfoFile << "*****************************" << std::endl;
	dirInfoFile << std::endl;
	dirInfoFile << GetBodybuildingWeekDateCodeInfo(
			currentDayWeek.weekBeginDTI, currentDayWeek.weekEndDTI);
	dirInfoFile << std::endl;
	dirInfoFile << std::endl;
	dirInfoFile << "Files:" << std::endl;
	dirInfoFile << "---------" << std::endl;
	
	int numberOfFilesCopied = filesCopied.size();
	for (int i = 0; i < numberOfFilesCopied; i++) {
		dirInfoFile << "> " << filesCopied[i] << std::endl;
	}
	
	dirInfoFile << std::endl;
	dirInfoFile << "* WBT - Weightlifting, Bodybuilding, Training" << std::endl;
	
	// Close the info file when finished editing
	dirInfoFile.close();
}

bool writeDecodedBinaryWeeklyRoutinesFilesFromMemory(std::vector<std::string> files,
		std::vector<std::vector<std::string> > binaryFileVector,
		std::string outFile,
		std::string fileName,
		struct DayAndWeekInfo currentDayWeek) {
	
	if (!files.size()) {
		// Return the value
		return false;
	}
	
	int filePosition = 0;
	
	int numberOfFiles = files.size();
	for (int i = 0; i < numberOfFiles; i++) {
		if (files[i] == fileName) {
			filePosition = (i + 1);
			break;
		}
	}
	
	if (!filePosition) {
		// No matching file found
		
		// Return the value
		return false;
	}

	fstream out;
	out.open(outFile.c_str(), ios::out | ofstream::binary);
	if (!out.is_open()) {
		out.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	std::string line;
	int currentLine = 0;
	
	while (currentLine <= filePosition) {
		line = binaryFileVector[0][currentLine];
		// In case bad value given, check for EOF
		if (currentLine < filePosition) {
			line.clear();
			currentLine++;
			continue;
		}
		std::string ibsonComma;
		int begPos = 0;
		while (begPos < line.size()) {
			ibsonComma = line.substr(begPos, 3);
			begPos += 4;
			int cNum;
			std::istringstream streamNum(ibsonComma);
			streamNum >> cNum;
			std::string secureReadVal = config.getValue("DATAPROC.Application.SecureRead");
			secureReadVal = secureReadVal.substr(2,secureReadVal.size());
			int srv;
			std::istringstream streamReadVal(secureReadVal);
			streamReadVal >> srv;
			out << (char)(cNum - srv);
		}
		
		// Get out of the loop
		break;
	}
	
	out.close();
	
	fstream in;
	in.open(outFile.c_str());//copied, left for visual aid: "ios::in", remove??
	if (!in.is_open()) {
		in.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	std::string lineData;
	std::vector<std::string> allFileLineData;
	while (true) {
		getline(in, lineData);
		std::string::size_type n = lineData.find("{YEAR}");
		if (n != std::string::npos)
			lineData.replace(n, 6, currentDayWeek.dayDTI.pui_gdt_str_year);
		n = lineData.find("{WEEK_NUMBER}");
		if (n != std::string::npos)
			lineData.replace(n, 13, currentDayWeek.dayDTI.pui_gdt_str_week);
		std::string dateDataForVariables = (currentDayWeek.dayDTI.pui_gdt_str_mday + " " +
					currentDayWeek.dayDTI.pui_gdt_str_mon + " " +
					currentDayWeek.dayDTI.pui_gdt_str_year + " at " + 
					currentDayWeek.dayDTI.pui_gdt_str_hour + ":" +
					currentDayWeek.dayDTI.pui_gdt_str_min + ":" +
					currentDayWeek.dayDTI.pui_gdt_str_sec);
		n = lineData.find("{MODIFIED_DATE}");
		if (n != std::string::npos)
			lineData.replace(n, 15, dateDataForVariables);
		n = lineData.find("{ACCESSED_DATE}");
		if (n != std::string::npos)
			lineData.replace(n, 15, dateDataForVariables);
		n = lineData.find("{CREATED_DATE}");
		if (n != std::string::npos)
			lineData.replace(n, 14, dateDataForVariables);
		allFileLineData.push_back(lineData);
		// Because the data will not get modified if this EOF check/break-point is
		// added at the top, we must move it to the bottom
		if (in.tellg() == EOF) {
			in.close();
			break;
		}
	}
	
	in.close();
	
	//ofstream out;
	out.open(outFile.c_str(), ios::out | ios::trunc | ofstream::binary);
	if (!out.is_open()) {
		out.close();
		
		// Return the value
		return false; // Write failed, we cannot proceed
	}
	
	for (int finalOut = 0; finalOut < allFileLineData.size(); finalOut++)
		out << allFileLineData[finalOut] << std::endl;
	
	out.close();
	
	// Return the value
	return true;
}