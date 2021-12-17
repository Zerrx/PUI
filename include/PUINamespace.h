#ifndef PUINAMESPACE_H
#define PUINAMESPACE_H

// Includes
#include <map>
#include <string>
#include <vector>

// Declaration
namespace PUI {
	extern std::map<std::string, std::vector<std::string> > appData;
	extern std::string applicationPath;
};
#endif