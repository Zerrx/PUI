#ifndef PUIDATETIME_H
#define PUIDATETIME_H

// Includes
#include <string>
#include <vector>

struct tm getSpecifiedDateTimeSI(int tm_sec, int tm_min,
		int tm_hour, int tm_mday, int tm_mon, int tm_year,
		int tm_wday);

int _getValidMDay(int tm_mday, int tm_mon, int tm_year);

std::vector<int> _getValidMDaysArray(int year);

void waitOneMoment();

struct PUIVisibleDTS {
	int pui_gdt_sec;
	std::string pui_gdt_str_sec;
	int pui_gdt_min;
	std::string pui_gdt_str_min;
	int pui_gdt_hour;
	std::string pui_gdt_str_hour;
	int pui_gdt_mday;
	std::string pui_gdt_str_mday;
	int pui_gdt_mon;
	std::string pui_gdt_str_mon;
	std::string pui_gdt_str_int_mon;
	int pui_gdt_year;
	std::string pui_gdt_str_year;
	int pui_gdt_wday;
	std::string pui_gdt_str_wday;
	std::string pui_gdt_str_int_wday;
	int pui_gdt_yday;
	std::string pui_gdt_str_yday;
	int pui_gdt_week;
	std::string pui_gdt_str_week;
	int pui_gdt_begin_week_date;
	int pui_gdt_end_week_date;
	int pui_gdt_isdst;
	std::string pui_gdt_str_isdst;
};

struct DayAndWeekInfo {
	struct PUIVisibleDTS dayDTI;
	struct PUIVisibleDTS weekBeginDTI;
	struct PUIVisibleDTS weekEndDTI;
};

struct DayAndWeekInfo getDayAndWeekInfo(struct tm dateTime);

struct PUIVisibleDTS getPUIDTStruct(struct tm dateTimeStructure);

int getProperWeekDayNumber (int wday_num);

std::string getPaddedDigitString(int dig, int padLength);

std::string getMonthName(int num);

std::string getWeekDayName(int num);

int getWeekNumber(int num);

std::string getIsDaylightSavingsTime(int isDST);

std::string convertIntToString(int num);

std::string convertDoubleToString(double num);

std::vector<int> getWeekBeginEndYDay(int week_num);

std::vector<int> getWeekBeginEndDate(int yday, int year);

int getDaysWeekBeginEnd(int week, int yday, std::string sinceOrUntil);

struct tm AddSubMakeADate(int date);

struct tm GetCurrentDateTM(void);

std::string getWBTCode(std::string begin_int_wday,
		std::string begin_mday, std::string begin_int_mon,
		std::string begin_year, std::string end_int_wday,
		std::string end_mday, std::string end_int_mon,
		std::string end_year);

/* *********************************************************************
	Get Input Date in DWBT code Format (DNDDMMYYYY) : (DayNumber-Day-MonthNumber-YearNumber)
	--------------------------------------------------------------------------------------------------
		1. Input DWBT string
		2. Validate
			2-.  On validation failure: return "-1"
			2-A. length == 10 || length == 11 (AND "D" or "d" as first character)
			2-B. DayNumber >= 1 && <= 7
			2-C. Month >= 1 && <= 12
				 2-C.1. DaysInMonth = getDaysInMonth
			2-D. Day >= 1 && <= DaysInMonth (2-C.1)
			2-E. YearNumber >= 2014
		3. Return int std::vector to make a date
********************************************************************* */

std::vector<int> getDWBTDate(std::string DWBTCode);

std::string getDWBTCode(std::string begin_int_wday,
		std::string begin_mday, std::string begin_int_mon,
		std::string begin_year);

struct PUIVisibleDTS getDate();

std::string getValidColonHMSTime(std::string colonHMS);

bool isValidTime(std::string time);

std::string getTimePeriod(std::string time);

#endif
