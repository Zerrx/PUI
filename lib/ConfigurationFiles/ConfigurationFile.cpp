// Regular includes
#include <errno.h>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <cstdio>
// Application includes
#include "..\..\include\ConfigurationFile.h"

template <class T> bool from_string(T &t,
		const std::string &s,
		std::ios_base &(*f)(std::ios_base&)) {
	std::istringstream iss(s);
	return !(iss>>f>>t).fail();
}

class StringUtil {
	public:
		StringUtil() {}
		~StringUtil() {}
		// Find the given std::string in the source
		// std::string and replace it with the
		// "replace" std::string, everywhere
		// instances of that std::string exist.
		static void findAndReplace(std::string &source,
				const std::string &find,
				const std::string &replace) {
			size_t j;
			for (; (j = source.find(find)) != std::string::npos;) {
				source.replace(j, find.length(), replace);
			}
		}
		// The following function returns a
		// std::string with all-uppercase characters.
		static std::string makeUpper(const std::string &inString) {
			std::string temp = inString;
			transform(temp.begin(), temp.end(),
					temp.begin(), ::toupper);
			return temp;
		}
		// The following function returns a
		// std::string with all-lowercase characters.
		static std::string makeLower(const std::string &inString) {
			std::string temp;
			transform(temp.begin(), temp.end(),
					temp.begin(), ::tolower);
			return temp;
		}
		static bool contains(const std::string &source,
				const char *find) {
			return (0 != strstr(source.c_str(),find));
		}
		static std::string pad(const std::string &inString,
				char padchar, int length) {
			std::string outString = inString;
			for (int i = (int)outString.length(); i < length; ++i)
				outString += padchar;
			return outString;
		}
		// Trim the given characters from the
		// beginning and end of a std::string.
		// the default is to trim whitespace.
		// If the std::string is empty or contains
		// only the trim characters, an empty
		// std::string is returned.
		static std::string trim(const std::string &inString,
				const std::string &trimString = std::string(" \t\n")) {
			if (trimString.size() == 0)
				return inString;
			std::string temp = "";
			std::string::size_type begpos = inString.find_first_not_of (trimString);
			if (begpos == std::string::npos) {
				return temp;
			} else {
				std::string::size_type endpos = inString.find_last_not_of (trimString);
				temp = inString.substr(begpos, ((endpos - begpos) + 1));
			}
			return temp;
		}
		// Convert the std::string to an int. Note that a std::string exception is thrown if
		// it is invalid.
		static int toInt(const std::string &myInString) {
			int i = 0;
			std::string inString = trim(myInString);
			if(!from_string<int>(i, inString, std::dec)) {
				std::string exceptionText = "StringUtils::toInt() - Not an integer: " + inString;
				throw exceptionText;
			}
			// Time to run some more checks.
			for (unsigned int j = 0; j < inString.length(); j++) {
				if (!isNumeric(inString[j])) {
					if ((j == 0) && (inString[j] == '-')) {
						continue;
					} else {
						std::string exceptionText = "StringUtils::toInt() - Not an integer: " + inString;
						throw exceptionText;
					}
				}
			}
			return (i);
		}
		// Convert the std::string to an int. Note:
		// A std::string exception is thrown if
		// it is invalid.
		static float toFloat(const std::string &myInString) {
			float f = 0;
			std::string inString = trim(myInString);
			if(!from_string<float>(f, inString, std::dec)) {
				std::string exceptionText = "StringUtils::toFloat() - Not a float: " + inString;
				throw exceptionText;
			}
			// Now it runs some more checks.
			int dec_count = 0;
			for (unsigned int j = 0; j < inString.length(); j++) {
				if (!isNumeric(inString[j])) {
					if ((j == 0) && (inString[j] == '-')) {
						continue;
					} else if (inString[j] == '.') {
						dec_count++;
						if (dec_count > 1) {
							std::string exceptionText = ("StringUtils::toFloat() - Not a float: " + inString);
							throw exceptionText;
						}
						continue;
					} else {
						std::string exceptionText = "StringUtils::toFloat() - Not a float: " + inString;
						throw exceptionText;
					}
				}
			}
			return (f);
		}
		// Returns true if the character is numeric.
		static bool isNumeric(char c) {
			return (('0' <= c) && (c <= '9'));
		}
		// Replace environment variables in the std::string with their values.
		// Note: environment variables must be of the form ${ENVVAR}.
		static std::string substituteEnvVar(const std::string &myInString) {
			std::string outString = "";
			char variable[512];
			const char *s = myInString.c_str();
			while(*s != 0) {
				if ((*s == '$') && (*(s + 1) == '{')) {
					// When you've found beginning of variable, find the end.
					strcpy(variable, (s + 2));
					char *end = strchr(variable, '}');
					if (end) {
						*end = '\0';
						char *cp = (char *)getenv(variable);
						if (cp)
							outString += (char *)getenv(variable);
						s = strchr(s,'}');
					} else {
						outString += *s;
					}
				} else {
					outString += *s;
				}
				s++;
			}
			return outString;
		}
};

ConfigurationFile::ConfigurationFile(void) {}

ConfigurationFile::ConfigurationFile(const char *strConfigFile) {
	if (strConfigFile)
		m_sConfigFile = strConfigFile;
}

ConfigurationFile::~ConfigurationFile() {}

bool ConfigurationFile::read(const char *strConfigFile) {
	if (strConfigFile) {
		m_sConfigFile = strConfigFile;
		m_in.open(m_sConfigFile.c_str(), ios::in);
		if (m_in.fail()) {
			return false;
		}
		while (!m_in.eof()) {
			//--------------------------------------------------------
			// Get a token and value.
			// This gives values to member vars: m_token and m_value.
			//----------------------------------------------------------
			getTokenAndValue();
			if (m_token.length())
				m_ConfigEntries.insert(stringPair(m_token, m_value));
			m_token.clear();
			m_value.clear();
		}
		m_in.close();
		return true;
	} else
		return false;
}

bool ConfigurationFile::read() {
	m_in.open(m_sConfigFile.c_str(), ios::in);
	if (m_in.fail()) {
		return false;
	}
	while (!m_in.eof()) {
		//--------------------------------------------------------
		// Get a token and value.
		// This gives values to member vars: m_token and m_value.
		//----------------------------------------------------------
		getTokenAndValue();
		if (m_token.length())
			m_ConfigEntries.insert(stringPair(m_token, m_value));
		m_token.clear();
		m_value.clear();
	}
	m_in.close();
	return true;
}

// BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT
bool ConfigurationFile::reset(void) {
	// Clear the data
	m_token.clear();
	m_value.clear();
	m_ConfigEntries.clear();
	
	// Reset the data
	
	// Return the value
	return read();
}

bool ConfigurationFile::save(void) {
	// Open the config file
	m_in.open(m_sConfigFile.c_str(), ios::in);
	// Failed to open? Tell the user the write failed!
	if (m_in.fail())
		return false;
	// Declare a temporary string to hold the line data
	// and to push the data to the config file vector
	std::string tmpStr;
	std::vector<std::string> configFileData;
	// Need to know the positions in the configFileData
	// (corresponding to line numbers) of data wherein
	// token=value pairs are found!
	std::vector<int> pairs;
	int lineNum = 0;
	while (getline(m_in, tmpStr)) {
		lineNum++;
		// Because spaces may precede comments, check for isComment
		bool isComment = false;
		for (int i = 0; i < tmpStr.size(); i++) {
			if (isspace(tmpStr[i])) {
				continue;
			} else if (tmpStr[i] == *"#") {
				isComment = true;
				break;
			} else
				break; // Not a comment
		}
		std::string::size_type equalPos = tmpStr.find('=');
		configFileData.push_back(tmpStr);
		if ((!isComment) && (equalPos != std::string::npos))
			pairs.push_back(lineNum - 1);
		tmpStr.clear();
	}
	m_in.close();
	
	// Open the config file...again...but truncate and output
	m_in.open(m_sConfigFile.c_str(), ios::trunc | ios::out);
	// Failed to open? Tell the user the write failed!
	// Return the value
	if (m_in.fail())
		return false;
	int sizeOfPairs = pairs.size();
	for (int i = 0; i < lineNum; i++) {
		bool iIsPair = false;
		for (int inPair = 0; inPair < sizeOfPairs; inPair++) {
			if (pairs[inPair] == i) {
				iIsPair = true;
				break;
			}
		}
		if (!iIsPair)
			m_in << configFileData[i] << std::endl;
		else {
			tmpStr = configFileData[i];
			std::string t = tmpStr.substr(0, tmpStr.find('='));
			int sizeOfT = t.size();
			std::string token = "";
			for (int j = 0; j < sizeOfT; j++) {
				if (!isspace(t[j]))
					token += t[j];
			}
			// Get the value before adding tabs so we can output
			// any data the user has populated the token with
			std::string value = getValue(token.c_str());
			// Add tabs for "nested" items
			for (int ch = 0; ch < sizeOfT; ch++) {
				if (t[ch] == '\t')
					token = ("\t" + token);
				else
					break;
			}
			m_in << token << " = " << value << std::endl;
		}
		tmpStr.clear();
	}
	
	// Add new values to the configuration file
	int totalNewKeyPairs = newKeyPairs.size();
	if (totalNewKeyPairs) {
		m_in << std::endl << std::endl << std::endl; 
		for (int ii = 0; ii < totalNewKeyPairs; ii++) {
			m_in << newKeyPairs[ii] << " = " << getValue(newKeyPairs[ii].c_str()) << std::endl;
		}
		// Clear any new key pairs once added, so we do not add them twice
		newKeyPairs.clear();
	}
	
	// Close the file
	m_in.close();
	
	// If we made it here, writing was successful!
	
	// Return the value
	return true;
}
// BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT BRETT

void ConfigurationFile::getTokenAndValue(void) {
	char token[1024];
	char ch;
	bool found_equal = false;
	int i = 0;
	eatWhiteAndComments();
	while(!(m_in.get(ch)).fail()) {
		/*
		ORIGINAL:
		-----------------
		if (ch != '\t') {
			if ((ch == '=') || (ch == ' ') || (ch == '\n') ||
					(ch == '\r') || (ch == '\t')) {
				if (ch == '=')
					found_equal = true;
				break;
			}
			token[i++] = ch;
		}
		*/
		if (ch == '=') {
			found_equal = true;
			break;
		} else if ((ch == ' ') || (ch == '\n') ||
				(ch == '\r') || (ch == '\t')) {
			break;
		}
		token[i++] = ch;
	}
	if (i == 0) {
		// It didn't find a token, in this case.
		m_token = "";
		m_value = "";
		return;
	}
	// Null-terminate the token that was found.
	token[i++] = '\0';
	m_token = token;
	// TODO:: SHOULD WE REMOVE?? makeLower(m_token);
	makeLower(m_token);
	// Advance to the equal sign, if need be.
	if (!found_equal) {
		if (!advanceToEqualSignOnLine()) {
			// The token had no value.
			m_token = "";
			m_value = "";
			return;
		}
	}
	// Get the token's value.
	i = 0;
	char c = eatWhiteAndComments(false);
	if (c != '\n') {
		i = 0;
		while(!(m_in.get(ch)).fail()) {
			if ((ch == '\t') || (ch == '\r') || (ch == '\n') || (ch == '#')) {
				while (ch != '\n') {
					if (m_in.get(ch).fail())
						break;
				}
				break;
			} else {
				token[i++] = ch;
			}
		}
	}
	if (i == 0) {
		// This token had no value.
		m_value = "";
	} else {
		token[i++] = '\0';
		m_value = token;
		// Remove leading/trailing spaces.
		m_value = StringUtil::trim(m_value);
		// Strip leading and trailing quotes, if there are any.
		if (m_value[0] == '"')
			m_value = m_value.substr(1);
		if (m_value[(m_value.length() - 1)] == '"')
			m_value = m_value.substr(0, (m_value.length() - 1));
	}
}

bool ConfigurationFile::advanceToEqualSignOnLine() {
	char ch;
	bool found_equal = false;
	while (!(m_in.get(ch)).fail()) {
		if ((ch == '\r') || (ch == '\n'))
			break;
		if (ch == '=') {
			found_equal = true;
			break;
		}
	}
	return found_equal;
}

char ConfigurationFile::eatWhiteAndComments(bool traverse_newlines) {
	char ch;
	bool in_comment = false;
	while (!(m_in.get(ch)).fail()) {
		if (ch == '#')
			in_comment = true;
		else if (ch == '\n') {
			in_comment = false;
			if (!traverse_newlines) {
				return(ch); // Stop eating.
			}
		} else if ((!in_comment) && (ch != ' ') && (ch != '\t') && (ch != '\r')) {
			m_in.putback(ch);
			return 0;
		}
	}
	return 0;
}

// TODO:: SHOULD WE REMOVE?? makeLower(m_token);
void ConfigurationFile::makeLower(std::string &inString) {
	for(unsigned i = 0; i < inString.size(); i++) {
		inString[i] = tolower(inString[i]);
	}
}

bool ConfigurationFile::hasValue(const char *key) {
	bool bRet = false;
	std::string sKey = key;
	// TODO:: SHOULD WE REMOVE?? makeLower(m_token);
	makeLower(sKey);
	if (m_ConfigEntries.find(sKey.c_str()) != m_ConfigEntries.end()) {
		bRet = true;
	}
	return bRet;
}

std::string ConfigurationFile::getValue(const char *key) {
	std::string sKey = key;
	// TODO:: SHOULD WE REMOVE?? makeLower(m_token);
	makeLower(sKey);
	if (m_ConfigEntries.find(sKey.c_str()) != m_ConfigEntries.end()) {
		std::map<std::string, std::string>::iterator iter;
		iter = m_ConfigEntries.find(sKey.c_str());
		return (*iter).second;
	}
	return "";
}

void ConfigurationFile::setValue(const char *key, const char *value) {
	if (!hasValue(key)) {
		newKeyPairs.push_back((std::string)key);
		m_ConfigEntries.insert(stringPair(key, ""));
	}
	std::string sKey = key;
	// TODO:: SHOULD WE REMOVE?? makeLower(m_token);
	makeLower(sKey);
	m_ConfigEntries[sKey] = value;
}



/* ****************************************************************
bool ConfigurationFile::read(const char *strConfigFile) {
	if ((strConfigFile) && (!m_sConfigFile.size()))
		m_sConfigFile = strConfigFile;
	else
		return false;
	
	// Must reset the value here...
	numberOfEntries = 0;
	
	m_in.open(m_sConfigFile.c_str(), ios::in);
	if (m_in.fail()) {
		return false;
	}
	while (!m_in.eof()) {
		//--------------------------------------------------------
		// Get a token and value.
		// This gives values to member vars: m_token and m_value.
		//----------------------------------------------------------
		getTokenAndValue();
		if (m_token.length()) {
			m_ConfigEntries.insert(stringPair(m_token, m_value));
			numberOfEntries++;
		}
		m_token.clear();
		m_value.clear();
	}
	m_in.close();
	return true;
}

int ConfigurationFile::getSize(void) {
	// TODO: Added, should we remove???
	return numberOfEntries;
}

map<std::string, std::string> ConfigurationFile::getConfigList(void) {
	return m_ConfigEntries;
}

**************************************************************** */