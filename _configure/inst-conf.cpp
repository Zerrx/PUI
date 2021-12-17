// Basic functions libraries
// System and Special includes
#include <cstdio>
#include <cstdlib> // For system processes such as system("pause");
#include <windows.h>
// Regular includes
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <list>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Application includes
#include "..\include\conf.h"
#include "..\include\PUINamespace.h"
#include "..\include\ConfigurationFile.h"

using namespace std;

using namespace PUI;

std::string modifyConfigurationFile(int lItem);

void invalidEntry(void);

// Main function
int main(int argc, char* argv[]) {
	// Introduce the configuration wizard to the user
	cout << "**********************************************************************" << endl;
	cout << "Welcome to PUI's Installation Configuration wizard!" << endl;
	cout << endl;
	cout << "This wizard will allow you select the appropriate modules to install," << endl;
	cout << "and configure your preferences for, PUI's application." << endl;
	cout << endl;
	cout << "If you elect to neglect configuring PUI at this time, you will be" << endl;
	cout << "returned to this wizard when you attempt to run PUI, as you must first" << endl;
	cout << "configure the application before running it." << endl;
	cout << endl;
	cout << "The following modules are available for inclusion:" << endl;
	cout << "  Animals       |  Farming" << endl;
	cout << "  Construction  |  Sports" << endl;
	cout << endl;
	cout << "The following sub-modules are available for inclusion:" << endl;
	cout << "  Animals           |  Farming" << endl;
	cout << "    Canine          |    Bovine" << endl;
	cout << "    Equine          |    Gardening" << endl;
	cout << "    Feline          |    Swine" << endl;
	cout << "  Construction      |  Sports" << endl;
	cout << "    Stair Stringer  |    Bodybuilding" << endl;
	cout << "                    |    Fishing" << endl;
	cout << "                    |    Running" << endl;
	cout << endl;
	cout << "The following preferences/settings are configurable:" << endl;
	cout << "  Display: Size (Width) in Number of Characters" << endl;
	cout << "  Display: Number of Lines (Length)" << endl;
	cout << endl;
	cout << "The following module-specific preferences/settings are configurable:" << endl;
	cout << "  Sports directory, which will house \"Bodybuilding\", \"Fishing\", and" << endl;
	cout << "    \"Running\"." << endl;
	cout << "  Construction directory, which will house \"Stair Stringers\"." << endl;
	cout << endl;
	cout << endl;
	cout << "To go back, type \"back;\", to go forward, simply hit the 'Enter' key." << endl;
	cout << "**********************************************************************" << endl;
	
	// Before anything else...load the configuration information
	if (!PUI::config.read("PUI.cfg")) {
		std::cout << "\n\nSomething went wrong reading the configuration file...\n\n";
		
		// Return the value
		return 1;
	}
	
	// Set the Installation Directory
	if (PUI::config.getValue("Application.InstallDirectory") == "") {
		// TODO: Ensure PUI.exe, when calling this executable, passes this executable's path in the command line arguments for CreateProcess at the FIRST position (position 0) in the executable's arguments list
		std::string tmp = (std::string)argv[0];
		PUI::applicationPath = tmp.substr(0, tmp.find_last_of("\\") + 1);
		PUI::config.setValue("Application.InstallDirectory", PUI::applicationPath.c_str());
	}
	
	int pos = 1;
	while (true) {
		// Just in case we get this far.
		if (pos == 0)
			pos++;
		std::string res = modifyConfigurationFile(pos);
		if (res == ";;FINISHED;;") break;
		else if (res == "back;") pos--;
		else pos++;
		// Repeat until quit
	}
	
	PUI::config.save();
	
	cout << "\n\nPress \"enter\" to continue...";
	cin.get();
	
	// Return the value
	return 0;
}

/*
if (config.getValue("Module.") == "true") {
	while (true) {
		input.clear();
		cout << "\nWould you like to include the \"\" sub-module? (y/Y or n/N)    ";
		getline(cin, input);
		if ((input == "y") || (input == "Y")) {
			PUI::config.setValue("", "");
			break;
		} else if ((input == "n") || (input == "N")) {
			PUI::config.setValue("", "");
			break;
		} else if (input == "back;") {
			result = "back;";
			break;
		}
		// Else, invalid entry given
		invalidEntry();
	}
}
*/

std::string modifyConfigurationFile(int lItem) {
	std::string result = "";
	std::string input;
	switch (lItem) {
		case 1 :
			while (true) {
				input.clear();
				cout << "\nWould you like to include the \"Animals\" module? (y/Y or n/N)    ";
				getline(cin, input);
				if ((input == "y") || (input == "Y")) {
					PUI::config.setValue("Module.Animals", "true");
					break;
				} else if ((input == "n") || (input == "N")) {
					PUI::config.setValue("Module.Animals", "false");
					// Set all sub-modules to false, in case the user has went
					// back and selected to not include the module
					PUI::config.setValue("Module.Animals.Canine", "false");
					PUI::config.setValue("Module.Animals.Equine", "false");
					PUI::config.setValue("Module.Animals.Feline", "false");
					break;
				}
				// Else, invalid entry given
				invalidEntry();
			}
			break;
		case 2 :
			if (config.getValue("Module.Animals") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Canine\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Animals.Canine", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Animals.Canine", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 3 :
			if (config.getValue("Module.Animals") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Equine\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Animals.Equine", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Animals.Equine", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 4 :
			if (config.getValue("Module.Animals") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Feline\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Animals.Feline", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Animals.Feline", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 5 :
			while (true) {
				input.clear();
				cout << "\nWould you like to include the \"Construction\" module? (y/Y or n/N)    ";
				getline(cin, input);
				if ((input == "y") || (input == "Y")) {
					PUI::config.setValue("Module.Construction", "true");
					break;
				} else if ((input == "n") || (input == "N")) {
					PUI::config.setValue("Module.Construction", "false");
					// Set all sub-modules to false, in case the user has went
					// back and selected to not include the module
					PUI::config.setValue("Module.Construction.StairStringer", "false");
					break;
				} else if (input == "back;") {
					result = "back;";
					break;
				}
				// Else, invalid entry given
				invalidEntry();
			}
			break;
		case 6 :
			if (config.getValue("Module.Construction") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Stair Stringer\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Construction.StairStringer", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Construction.StairStringer", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 7 :
			while (true) {
				input.clear();
				cout << "\nWould you like to include the \"Farming\" module? (y/Y or n/N)    ";
				getline(cin, input);
				if ((input == "y") || (input == "Y")) {
					PUI::config.setValue("Module.Farming", "true");
					break;
				} else if ((input == "n") || (input == "N")) {
					PUI::config.setValue("Module.Farming", "false");
					// Set all sub-modules to false, in case the user has went
					// back and selected to not include the module
					PUI::config.setValue("Module.Farming.Bovine", "false");
					PUI::config.setValue("Module.Farming.Gardening", "false");
					PUI::config.setValue("Module.Farming.Swine", "false");
					break;
				} else if (input == "back;") {
					result = "back;";
					break;
				}
				// Else, invalid entry given
				invalidEntry();
			}
			break;
		case 8 :
			if (config.getValue("Module.Farming") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Bovine\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Farming.Bovine", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Farming.Bovine", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 9 :
			if (config.getValue("Module.Farming") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Gardening\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Farming.Gardening", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Farming.Gardening", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 10 :
			if (config.getValue("Module.Farming") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Swine\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Farming.Swine", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Farming.Swine", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 11 :
			while (true) {
				input.clear();
				cout << "\nWould you like to include the \"Sports\" module? (y/Y or n/N)    ";
				getline(cin, input);
				if ((input == "y") || (input == "Y")) {
					PUI::config.setValue("Module.Sports", "true");
					break;
				} else if ((input == "n") || (input == "N")) {
					PUI::config.setValue("Module.Sports", "false");
					// Set all sub-modules to false, in case the user has went
					// back and selected to not include the module
					PUI::config.setValue("Module.Sports.Bodybuilding", "false");
					PUI::config.setValue("Module.Sports.Fishing", "false");
					PUI::config.setValue("Module.Sports.Running", "false");
					break;
				} else if (input == "back;") {
					result = "back;";
					break;
				}
				// Else, invalid entry given
				invalidEntry();
			}
			break;
		case 12 :
			if (config.getValue("Module.Sports") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Bodybuilding\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Sports.Bodybuilding", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Sports.Bodybuilding", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 13 :
			if (config.getValue("Module.Sports") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Fishing\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Sports.Fishing", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Sports.Fishing", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		case 14 :
			if (config.getValue("Module.Sports") == "true") {
				while (true) {
					input.clear();
					cout << "\nWould you like to include the \"Running\" sub-module? (y/Y or n/N)    ";
					getline(cin, input);
					if ((input == "y") || (input == "Y")) {
						PUI::config.setValue("Module.Sports.Running", "true");
						break;
					} else if ((input == "n") || (input == "N")) {
						PUI::config.setValue("Module.Sports.Running", "false");
						break;
					} else if (input == "back;") {
						result = "back;";
						break;
					}
					// Else, invalid entry given
					invalidEntry();
				}
			}
			break;
		default :
			result = ";;FINISHED;;";
	}
	
	// Return the value
	return result;
}

void invalidEntry(void) {
	cout << "\nInvalid entry! Only 'y', 'Y', 'n', or 'N' acceptable!";
}

/* **************************************************
g++ -o ../PUI-conf.exe inst-conf.cpp ../lib/ConfigurationFiles/conf.cpp ../lib/ConfigurationFiles/ConfigurationFile.cpp ../lib/PUI/PUINamespace.cpp

/////////////////////////////////////////////////////
	PUI::config.setValue("FLINKCING-DING-DONGALDKAK", "FAORAKKAKAKAKAKKAAKKAKAKAKKAKAK");
	cout << endl << endl << endl << endl << PUI::config.hasValue("FLINKCING-DING-DONGALDKAK") << endl << endl << endl << endl;
/////////////////////////////////////////////////////
	
************************************************** */















/*-----------------------------------------------------------------------------------------------------------------
//////////////////////////////
REQUIRED
//////////////////////////////
Module.Animals
Module.Animals.Canine
Module.Animals.Equine
Module.Animals.Feline
Module.Construction
Module.Construction.StairStringer
Module.Sports
Module.Sports.Bodybuilding
Module.Sports.Fishing
Module.Sports.Running
Module.Farming
Module.Farming.Bovine
Module.Farming.Swine
Module.Farming.Gardening
Application.DisplaySize
Application.DisplayLineNumbers
Application.PrimaryDirectory



//////////////////////////////
REQUIRED WHEN MODULE USED
//////////////////////////////
Module.Sports.Directory
Module.Sports.Bodybuilding.Directory
Module.Construction.Directory
Module.Construction.StairStringer.Directory



//////////////////////////////
FILLED WHEN INSTALLED
//////////////////////////////
Application.InstallDirectory
Application.HelpDirectory
DATAPROCF.Application.Introduction
DATAPROCF.Application.Forms.DataSheets
DATAPROC.Application.SecureRead
DATAPROCF.Module.Sports.Bodybuilding.DataSheets
-----------------------------------------------------------------------------------------------------------------*/