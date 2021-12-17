#ifndef BODYBUILDING_H
#define BODYBUILDING_H

//Includes
#include <string>

bool Bodybuilding();

int bodybuildingInputHandler(void);

bool isValidBodybuildingCommand(std::string command);

int executeBodybuildingCommand(std::string command);

#endif