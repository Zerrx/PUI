#ifndef _CONFIGURATIONFILE_H_
#define _CONFIGURATIONFILE_H_

// Regular includes
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <list>

using namespace std;

class ConfigurationFile {
	// Public functions
	public:
		ConfigurationFile(void);
		ConfigurationFile(const char *strFileName);
		virtual ~ConfigurationFile(void);
		bool read(const char *strFileName);
		bool read(void);
		bool hasValue(const char *key);
		std::string getValue(const char *key);
		void setValue(const char *key, const char *value);
		bool save(void);
		bool reset(void);
	// Protected functions
	protected:
		virtual void getTokenAndValue();
		virtual char eatWhiteAndComments(bool traverse_newlines = true);
		virtual bool advanceToEqualSignOnLine();
		virtual void makeLower(std::string &inString);
	// Protected variables
	protected:
		fstream m_in;
		std::string m_token;
		std::string m_value;
		std::string m_sConfigFile;
		typedef pair<std::string, std::string> stringPair;
		map<std::string, std::string> m_ConfigEntries;
		std::vector<std::string> newKeyPairs;
};

#endif