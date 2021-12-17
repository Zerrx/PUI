// Basic functions libraries
// System and Special includes
#include <cstdio>
#include <cstdlib>
#include <windows.h>
// Regular includes
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// File I/O and Directory Manipulation libraries
#include <fstream>
#include <direct.h>
// Date/Time functionality
#include <ctime>
#include <iomanip>
#include <locale>
// Application includes
#include "..\..\include\DateTime.h"
#include "..\..\include\Interface.h"

std::string getWBTCode(std::string begin_int_wday,
		std::string begin_mday, std::string begin_int_mon,
		std::string begin_year, std::string end_int_wday,
		std::string end_mday, std::string end_int_mon,
		std::string end_year) {
	
	std::string WBTCode = (begin_int_wday + begin_mday +
			begin_int_mon + begin_year + end_int_wday +
			end_mday + end_int_mon + end_year);
	
	// Return the value
	return WBTCode;
}

struct tm AddSubMakeADate(int date) {
	std::time_t cdt = std::time(NULL);
	struct tm *cdt1 = std::localtime(&cdt);
	cdt1->tm_mday += date;
	std::time_t bcydt = std::mktime(cdt1);
	struct tm *cdt2 = std::localtime(&bcydt);
	struct tm currentDateTimeList = *cdt2;
	
	// Return the value
	return currentDateTimeList;
}

struct tm GetCurrentDateTM(void) {
	std::time_t time_current = std::time(NULL);
	struct tm *_time_date_tm = std::localtime(&time_current);
	struct tm currentDateTimeList = *_time_date_tm;
	
	// Return the value
	return currentDateTimeList;
}

struct tm getSpecifiedDateTimeSI(int tm_sec, int tm_min,
		int tm_hour, int tm_mday, int tm_mon, int tm_year,
		int tm_wday) {
	/* ************************************************
	User Input Information
		int tm_sec,
			>> seconds after the minute – [0, 61]
			(until C99) / [0, 60] (since C99)
		int tm_min,
			>> minutes after the hour – [0, 59]
		int tm_hour,
			>> hours since midnight – [0, 23]
		int tm_mday,
			>> day of the month – [1, 31]
		int tm_mon,
			>> months since January – [0, 11]
		int tm_year,
			>> years since 1900
		int tm_wday,
			>> days since Sunday – [0, 6]
		int tm_yday,
			>> days since January 1 – [0, 365]
		int tm_isdst,
			>> Daylight Saving Time flag. The value
			is positive if DST is in effect, zero
			if not and negative if no information
			is available
		bool isPUIStandard
			>> Standard specifications of PUI require
			the day (wday) given conform to Monday as
			the start of the week, which contradicts
			C/C++ standards for tm. Therefore, when
			the value is true, conform to C/C++ so
			the getDateTimeString function can return
			the proper value as Monday - Sunday.
	************************************************ */
	
	// TODO: Add option to calculate the length of time from the
	// first workout.
	std::time_t cdt = std::time(NULL);
	//		WARNING!!! Ensure you reset the pointer for localtime
	//			back to the current time!!!
	
	// New tm to house pointer to Date/Time tm Information
	struct tm *qbt1 = localtime(&cdt);
	
	// Set the Date/Time information for the given Date/Time
	// Since dates coming from the user are to be in PUI format, we
	// must adjust certain values according to C/C++ specifications
	if (tm_sec != -1)
		qbt1->tm_sec = tm_sec;
	//std::cout << "\nSecond in Minute: " << qbt1->tm_sec << "\n";
	if (tm_min != -1)
		qbt1->tm_min = tm_min;
	//std::cout << "\nMinute in Hour: " << qbt1->tm_min << "\n";
	if (tm_hour != -1)
		qbt1->tm_hour = tm_hour;
	//std::cout << "\nHour of Day: " << qbt1->tm_hour << "\n";
	if (tm_mday != -1)
		qbt1->tm_mday = _getValidMDay(tm_mday, tm_mon, tm_year);
	//std::cout << "\nDay in Month: " << qbt1->tm_mday << "\n";
	if (tm_mon != -1)
		qbt1->tm_mon = --tm_mon; // Adjusted as user input will be in
		// the numeric range [1 - 12]
	//std::cout << "\nMonth of the Year: " << qbt1->tm_mon << "\n";
	if (tm_year != -1)
		qbt1->tm_year = (tm_year - 1900); // Adjusted as user input
		// will be the year
	//std::cout << "\nSelected Year: " << qbt1->tm_year << "\n";
	if (tm_wday != -1) {
		// Since program specifications require Monday as
		// the start of the year, ensure date given is
		// modified to conform to C++ specs and converted
		// in the getDateTimeString function
		if (tm_wday == 6)
			qbt1->tm_wday = 0;
		else
			qbt1->tm_wday = tm_wday++;
		//std::cout << "\nDay of Week: " << qbt1->tm_wday << "\n\n";
	}
	// Make the new date/time to a time_t time format for conversion
	// and calculation to ensure that localtime can process the info
	std::time_t bcydt = std::mktime(qbt1);
	// Process the new date and assign it to a temporary tm structure
	// for assignment to display to a user
	struct tm *tmp_tm = localtime(&bcydt);
	// Member to member copy allows the time to be copied into a
	// non-pointing tm
	struct tm beginCurrentYearDateTimeList = *tmp_tm;
	// Because tm does not allow anything other than pointers
	// for localtime and mktime, and the like, you must reset
	// the localtime function to the current Date/Time
	//		struct tm *resetLocalTimeFunction = localtime(&cdt);
	qbt1 = localtime(&cdt);
	
	// Return the value
	return beginCurrentYearDateTimeList;
}

int _getValidMDay(int tm_mday, int tm_mon, int tm_year) {
	// Initialize the _tm_mday value to return
	int _tm_mday = tm_mday;
	
	// Since argument values come from User Input, ensure
	// the proper Month is selected for validation
	--tm_mon;
	
	// Get the month's days
	std::vector<int> daysInMonths = _getValidMDaysArray(tm_year);
	
	// Set the input Month's day to a valid day:
	//		if the mday is greater than the month's days,
	//		mday is the last day of the month; if the mday
	//		is less than one (1), the first day of any month,
	//		set the value to the first day of the month
	if (tm_mday > daysInMonths[tm_mon])
		_tm_mday = daysInMonths[tm_mon];
	else if (tm_mday < 1)
		_tm_mday = 1;
	
	// Now that the correct day has been selected, return it to
	// be assigned to the tm_mday variable in the new tm struct
	
	// Return the value
	return _tm_mday;
}

std::vector<int> _getValidMDaysArray(int year) {
	// Initialize the Days in the Months vector/array
	std::vector<int> daysInMonths;
	
	// Mark the month's days by populating the
	// daysInMonths vector/array with standard loop
	int inc = 0;
	while (inc < 12) {
		int dim[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		daysInMonths.push_back(dim[inc]);
		inc++;
	}
	
	// Check if the year is divisible by 4 or 400 to set leap year
	// (that is, to increment the month of February accordingly)
	if ((((year % 4) == 0) && ((year % 100) != 0)) ||
		((year % 400) == 0))
		daysInMonths[1]++;
	
	// Return the value
	return daysInMonths;
}

struct PUIVisibleDTS getPUIDTStruct(struct tm dateTimeStructure) {
	// Declare a PUI-Visible Date/Time Structure to fill with data
	struct PUIVisibleDTS visibleDateTimeStructure;
	
	// Seconds: Needs populated and a two-digit display string
	visibleDateTimeStructure.pui_gdt_sec = dateTimeStructure.tm_sec;
	visibleDateTimeStructure.pui_gdt_str_sec =
			getPaddedDigitString(dateTimeStructure.tm_sec, 2);
	// Minutes: Needs populated and a two-digit display string
	visibleDateTimeStructure.pui_gdt_min = dateTimeStructure.tm_min;
	visibleDateTimeStructure.pui_gdt_str_min =
			getPaddedDigitString(dateTimeStructure.tm_min, 2);
	// Hours: Needs populated and a two-digit display string
	visibleDateTimeStructure.pui_gdt_hour = dateTimeStructure.tm_hour;
	visibleDateTimeStructure.pui_gdt_str_hour =
			getPaddedDigitString(dateTimeStructure.tm_hour, 2);
	// Month Day: Needs populated and a two-digit display string
	visibleDateTimeStructure.pui_gdt_mday = dateTimeStructure.tm_mday;
	visibleDateTimeStructure.pui_gdt_str_mday =
			getPaddedDigitString(dateTimeStructure.tm_mday, 2);
	// Month Int: Needs populated, needs converted to range [1-12],
	// needs converted to range [Jan-Dec], and needs a two-digit
	// display string
	visibleDateTimeStructure.pui_gdt_mon = ++dateTimeStructure.tm_mon;
	visibleDateTimeStructure.pui_gdt_str_mon =
			getMonthName(dateTimeStructure.tm_mon);
	visibleDateTimeStructure.pui_gdt_str_int_mon =
			getPaddedDigitString(dateTimeStructure.tm_mon, 2);
	// Year: Needs converted from since 1900 and a two-digit
	// display string
	visibleDateTimeStructure.pui_gdt_year =
			(dateTimeStructure.tm_year + 1900);
	visibleDateTimeStructure.pui_gdt_str_year =
			convertIntToString((dateTimeStructure.tm_year + 1900));
	// Week Day Int: Needs converted to numeric range [1-7], needs
	// converted to range [Mon-Sun], and needs a two-digit
	// display string
	visibleDateTimeStructure.pui_gdt_wday = getProperWeekDayNumber(dateTimeStructure.tm_wday);
	visibleDateTimeStructure.pui_gdt_str_wday =
			getWeekDayName(getProperWeekDayNumber(dateTimeStructure.tm_wday));
	visibleDateTimeStructure.pui_gdt_str_int_wday =
			getPaddedDigitString(getProperWeekDayNumber(dateTimeStructure.tm_wday), 2);
	// Year Day: Needs converted to range [1-366]
	visibleDateTimeStructure.pui_gdt_yday = ++dateTimeStructure.tm_yday;
	visibleDateTimeStructure.pui_gdt_str_yday =
			getPaddedDigitString(dateTimeStructure.tm_yday, 2);
	// Week: Needs converted from year day to week number
	// range [1-53], and needs a two-digit display string
	visibleDateTimeStructure.pui_gdt_week =
			getWeekNumber(dateTimeStructure.tm_yday);
	visibleDateTimeStructure.pui_gdt_str_week =
			getPaddedDigitString(visibleDateTimeStructure.pui_gdt_week,
					2);
	// Week Day Begin/End Date: Needs populated
		// Run this function once...
	std::vector<int> beginEndWeekDateInt =
			getWeekBeginEndDate(visibleDateTimeStructure.pui_gdt_yday,
					visibleDateTimeStructure.pui_gdt_year);
	visibleDateTimeStructure.pui_gdt_begin_week_date = beginEndWeekDateInt[0];
	visibleDateTimeStructure.pui_gdt_end_week_date = beginEndWeekDateInt[1];
	// Is Daylight Savings Time: Needs populated and needs
	// a display string
	visibleDateTimeStructure.pui_gdt_isdst = dateTimeStructure.tm_isdst;
	visibleDateTimeStructure.pui_gdt_str_isdst =
			getIsDaylightSavingsTime(dateTimeStructure.tm_isdst);
	
	// Return the value
	return visibleDateTimeStructure;
}

struct DayAndWeekInfo getDayAndWeekInfo(struct tm dateTime) {
	// Construct the Current Week Information
	struct DayAndWeekInfo dayAndWeekInformation;
	
	// Set the day's date/time information
	dayAndWeekInformation.dayDTI =
			getPUIDTStruct(dateTime);
	
	// Set the week's begin date/time information
	dayAndWeekInformation.weekBeginDTI =
			getPUIDTStruct(AddSubMakeADate(-1 *
					dayAndWeekInformation.dayDTI.pui_gdt_begin_week_date));
	
	// Set the week's end date/time information
	dayAndWeekInformation.weekEndDTI =
			getPUIDTStruct(AddSubMakeADate(
					dayAndWeekInformation.dayDTI.pui_gdt_end_week_date));
	
	// Return the value
	return dayAndWeekInformation;
}

int getProperWeekDayNumber (int wday_num) {
	// The incoming number is a range [0-6]
	// Initialize the variable which can only be valid [1-7]
	int weekDayNumber = 1;
	// If the number is a zero or less (Sunday), return the 7th day
	if (wday_num <= 0)
		weekDayNumber = 7;
	// If the number is greater than 6, return the 1st day
	else if (wday_num > 6)
		weekDayNumber = 1;
	// If not the previous, the value is correct
	else
		weekDayNumber = wday_num;
	// Return the value
	return weekDayNumber;
}

std::string getPaddedDigitString(int dig, int padLength) {
	std::string str_dig = convertIntToString(dig);
	int str_digLength = str_dig.length();
	while (str_dig.length() < padLength) {
		str_dig = ("0" + str_dig);
		str_digLength = str_dig.length();
	}
		
	// Return the value
	return str_dig;
}

std::string getMonthName(int num) {
	// Assumes input is valid 1-12 instead of 0-11
	std::vector<std::string> monthName;
	monthName.push_back("January");
	monthName.push_back("February");
	monthName.push_back("March");
	monthName.push_back("April");
	monthName.push_back("May");
	monthName.push_back("June");
	monthName.push_back("July");
	monthName.push_back("August");
	monthName.push_back("September");
	monthName.push_back("October");
	monthName.push_back("November");
	monthName.push_back("December");
	
	// If the given month number is greater than the length
	// of months in a year, assign the last month of a year
	if (num > 12)
		num = 11;
	// If the given month number is less than the first month
	// in a year, assign the first month of a year
	else if (num < 1)
		num = 0;
	// If the given month number is valid, ensure the
	// appropriate month is returned through decrementing
	else
		--num;
	
	// Return the value
	return monthName[num];
}

std::string getWeekDayName(int num) {
	// Function assumes PUI standards met first
	// (i.e. Mon-Sun instead of Sun-Mon)
	// Assumes input is valid 1-7 instead of 0-6
	std::vector<std::string> weekDayName;
	weekDayName.push_back("Monday");
	weekDayName.push_back("Tuesday");
	weekDayName.push_back("Wednesday");
	weekDayName.push_back("Thursday");
	weekDayName.push_back("Friday");
	weekDayName.push_back("Saturday");
	weekDayName.push_back("Sunday");
	
	// If the given day number is greater than the length
	// of days in a week, assign the last day of a week
	if (num > 7)
		num = 6;
	// If the given day number is less than the first day
	// in a week, assign the first day of a week
	else if (num < 1)
		num = 0;
	// If the given day number is valid, ensure the
	// appropriate day is returned through decrementing
	else
		--num;
	
	// Return the value
	return weekDayName[num];
}

int getWeekNumber(int num) {
	// Assumes input is valid 1-366 or 1-365 and
	// whether or not the year is a leap year instead of 0-365
	
	// Since there could be "53" weeks in a year, 52 plus 1 or
	// 2 days remaining, we must count the last 1 or 2 days as
	// a separate week. Instantiating the weekNumber to 1 here
	// saves on coding, as: we can add 52 weeks to the count
	// if the first valid num check shows true; we can leave
	// the value "unchanged" if the second valid num check
	// shows true; finally, we can just override the value if
	// neither of the checks show true.
	int weekNumber = 1;
	
	// If the given year day number is greater than the full
	// 52 weeks worth of days, or 364 days, set the weekNumber
	// to the 53 week
	if (num > 364)
		weekNumber += 52;
	// If the given year day number is less than the first day
	// of the second week, assign the first week
	else if (num < 8)
		weekNumber = 1;
	// If the given year day number is valid between the
	// possible perfect 52 weeks in a year, assign the week
	// number as the current week
	else {
		// If there is no remainder (modulus), do not set the
		// week number accidentally to the next week, as the
		// current day is in the week returned when divided
		if (!(num % 7))
			weekNumber = (num / 7);
		// If there is a remainder (modulus), set the week to
		// the next week, as the current day is in the middle
		// of the next week
		else
			weekNumber += (num / 7);
	}
	
	// Return the value
	return weekNumber;
}

std::string getIsDaylightSavingsTime(int isDST) {
	// Initialize the Daylight Savings Time display string
	std::string isDaylightSavingsTime = "Unavailable";
	
	// If isDST has a value of 1 or greater, then DST is active
	if (isDST >= 1)
		isDaylightSavingsTime = "Active";
	// If isDST has a value of 0, then DST is inactive
	else if (isDST == 0)
		isDaylightSavingsTime = "Inactive";
	// Any other values are already handled with the
	// initialization of the isDaylightSavingsTime variable
	
	// Return the value
	return isDaylightSavingsTime;
}

std::string convertIntToString(int num) {
	std::stringstream str_num;
	str_num << num;
	
	// Return the value
	return str_num.str();
}

std::string convertDoubleToString(double num) {
	std::stringstream str_num;
	str_num << num;
	
	// Return the value
	return str_num.str();
}

std::vector<int> getWeekBeginEndYDay(int week_num) {
	// Declare the day number for the beginning of 1st week
	int dayForWeekNum = 1;
	// Declare the week number for the previous week
	int previousWeekNum = (week_num - 1);
	// Declare the vector/array to house the week begin and end info
	std::vector<int> weekBeginEnd;
	
	// If the previousWeekNum == 0, week is week 1, as there was
	// no preceding week to calculate, and day is one
	if (previousWeekNum == 0) {
		weekBeginEnd.push_back(dayForWeekNum); // Begin
		weekBeginEnd.push_back((dayForWeekNum + 6)); // End
	} else {
		// If the previous week is not week 0 (aka 1), we simply need
		// to return 7 days times the previous week number to get the
		// beginning Year Day for week
		dayForWeekNum += (7 * previousWeekNum);
		weekBeginEnd.push_back(dayForWeekNum);
		weekBeginEnd.push_back((dayForWeekNum + 6));
	}
	
	// Return the value
	return weekBeginEnd;
}

std::vector<int> getWeekBeginEndDate(int yday, int year) {
	// Function Returns 2 digit sets of 1::
	// 1: Days since the beginning; 2: Days until the end
	// Declare the variable
	std::vector<int> weekBeginEndDate;
	
	// Declare and get the current week
	int week = getWeekNumber(yday);
	
	// Declare and get the days since the beginning of the week
	int sinceWeekBeginDays = getDaysWeekBeginEnd(week, yday, "since");
	
	// Declare and get the days until the end of the week
	int untilWeekEndDays = getDaysWeekBeginEnd(week, yday, "until");
	
	// Add the values to the variable
	weekBeginEndDate.push_back(sinceWeekBeginDays);
	weekBeginEndDate.push_back(untilWeekEndDays);
	
	// Return the value
	return weekBeginEndDate;
}

void waitOneMoment() {
	int i = 0;
	while (i < 100000000)
		i++;
}

int getDaysWeekBeginEnd(int week, int yday, std::string sinceOrUntil) {
	// Assumes input yday is valid 1-366 or 1-365 and
	// whether or not the year is a leap year instead of 0-365
	// Assumes week information already calculated
	// Declare the daysWeekBeginEnd to return
	int daysWeekBeginEnd = 0;
	
	// Declare the number of days week has by calculating week * 7
	int weeksDays = (week * 7);
	
	// Declare a tmp variable to hold differences
	int tmp = (yday % 7);
	
	//std::cout << "\nWeeksDays:: "<<weeksDays;	
	//std::cout << "\nYday:: "<<yday;	
	//std::cout << "\nTmp:: "<<tmp;	
	
	// If since selected, return number of days since week began
	if (sinceOrUntil == "since") {
		// Check if it is in the first week of the year
		if ((yday / weeksDays) < 1.0)
			daysWeekBeginEnd = --tmp;
		// If it is not the first week of the year, then the date
		// must be established as the last day of the week (6),
		// decremented because the date will return the last day
		// of the previous week
		else
			daysWeekBeginEnd = ((tmp == 0) ? 6 : (--tmp));
	} else if (sinceOrUntil == "until") {
		daysWeekBeginEnd = ((tmp == 0) ? 0 : (7 - tmp));
	}

	// Return the value
	return daysWeekBeginEnd;
}

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

std::vector<int> getDWBTDate(std::string DWBTCode) {
	// This function will always return a value, a -1 failure
	// Declare the int vector which holds the date numbers
	std::vector<int> DWBTDate;
	// Declare the string to re-hold the date numbers...we
	// need this if the user entered D-- before the date
	std::string DStrip;
	// Declare the length for pre-validation
	int codeLen = DWBTCode.size();
	
	// First validate the length of the input
	if (isClose(DWBTCode))
		DWBTDate.push_back(-2);
	else if (!((codeLen == 11) || (codeLen == 10)))
			DWBTDate.push_back(-1);
	else if ((codeLen == 11) &&
			(!((DWBTCode.substr(0,1) == "D") ||
			(DWBTCode.substr(0,1) == "d"))))
		DWBTDate.push_back(-1);
	else
		DStrip = DWBTCode.substr(((codeLen == 11) ? 1 : 0),
				DWBTCode.size());
	// Used a loop to allow for breaking
	while (!DWBTDate.size()) {
		// ^^ No data should be here, unless error, in which
		// case no further work needs to be done
		int dayWeekNum;
		int dayMonthNum;
		int monthNum;
		int yearNum;
		
		std::istringstream(DStrip.substr(0,2)) >> dayWeekNum;
		std::istringstream(DStrip.substr(2,2)) >> dayMonthNum;
		std::istringstream(DStrip.substr(4,2)) >> monthNum;
		std::istringstream(DStrip.substr(6,4)) >> yearNum;
		if ((!(dayWeekNum <= 7)) &&
				(!(dayWeekNum >= 1))) {
			DWBTDate.push_back(-1);
			break;
		} else if ((!(monthNum <= 12)) &&
				(!(monthNum >= 1))) {
			DWBTDate.push_back(-1);
			break;
		} else if (!(yearNum >= 2014)) {
			DWBTDate.push_back(-1);
			break;
		}
		// Set up validation for months
		std::vector<int> daysInMonths = _getValidMDaysArray(yearNum);
		if ((!(dayMonthNum <= daysInMonths[monthNum])) &&
				(!(dayMonthNum >= 1))) {
			DWBTDate.push_back(-1);
			break;
		} else {
			// Add the dates to the array
			DWBTDate.push_back(dayWeekNum);
			DWBTDate.push_back(dayMonthNum);
			DWBTDate.push_back(monthNum);
			DWBTDate.push_back(yearNum);
			break;
		}
	}
	
	// Return the value
	return DWBTDate;
}

std::string getDWBTCode(std::string begin_int_wday,
		std::string begin_mday, std::string begin_int_mon,
		std::string begin_year) {
	
	std::string DWBTCode = (begin_int_wday + begin_mday +
			begin_int_mon + begin_year);
	
	// Return the value
	return DWBTCode;
}

struct PUIVisibleDTS getDate() {
	// Get Date/Time
	struct PUIVisibleDTS userDate;
	// Get today's date as the initial date
	struct tm selectedDateInit = GetCurrentDateTM();
	struct PUIVisibleDTS selDate = getPUIDTStruct(selectedDateInit);
	// Loop to get date
	while (true) {
		// Check for quit first (user may have looped back!)
		if (selDate.pui_gdt_week == -2)
			break;
		// Boolean to determine if input date needs asking
		bool inputDate = false;
		std::string isGoodDate;
		std::cout << "\nIs this the correct date? (Y/y or N/n)\n\tDate: ";
		std::string dateStr = getDWBTCode(selDate.pui_gdt_str_int_wday,
				selDate.pui_gdt_str_mday, selDate.pui_gdt_str_int_mon,
				selDate.pui_gdt_str_year);
		std::cout << dateStr << std::endl << "\n> ";
		getline(std::cin, isGoodDate);
		if ((isGoodDate == "y") || (isGoodDate == "Y")) {
			inputDate = false;
			break;
		} else if ((isGoodDate == "n") || (isGoodDate == "N")) {
			inputDate = true;
		} else if (isClose(isGoodDate)) {
			// Use when getting date from the user
			selDate = getPUIDTStruct(selectedDateInit);
			selDate.pui_gdt_week = -2;
			break;
		} else {
			std::cout << "\nInvalid command...\n";
			continue;
		}
		
		// If the user says no, input the date
		if (inputDate) {
			// In case user enters bad date...loop
			while (true) {
				std::cout << "\nPlease input the date in DWBT format:\n\n> ";
				std::string userEnteredDate;
				getline(std::cin, userEnteredDate);
				if (isClose(userEnteredDate)) {
					// Use when getting date from the user
					selDate = getPUIDTStruct(selectedDateInit);
					selDate.pui_gdt_week = -2;
					break;
				}
				std::vector<int> userSelDate = getDWBTDate(userEnteredDate);
				if (userSelDate[0] == -1) {
					std::cout << "\nInvalid date entered, try again!\n";
					continue;
				}
				// Use when getting date from the user
				struct tm selD = getSpecifiedDateTimeSI(-1,-1,-1,
						userSelDate[1], userSelDate[2],
						userSelDate[3], userSelDate[0]);
				selDate = getPUIDTStruct(selD);
				break;
			}
		}
	}
	
	// Return the value
	return selDate;
}

std::string getValidColonHMSTime(std::string colonHMS) {
	// Create the h/m/s integers
	int hours;
	int minutes;
	int seconds;
	
	// Create the string variables to hold H/M/S individually
	std::string hoursStr;
	std::string minutesStr;
	std::string secondsStr;
	
	// Will always contain hours/minutes/seconds
	bool goForHours = true;
	bool goForMinutes = false;
	bool goForSeconds = false;
	
	const char *sep = ":";
	const char *digs = "0123456789";
	std::string::size_type b = colonHMS.find_first_of(digs, 0, 10);
	std::string::size_type n = colonHMS.find_first_of(sep, b, 1);
	// Loop through the array to find digits
	for (;;) {
		if (n == std::string::npos)
			n = colonHMS.size();
		// If b is not found, no data, break
		if (b == std::string::npos)
			break;
		if (goForHours) {
			std::istringstream(colonHMS.substr(b,n)) >> hours;
			goForHours = false;
			goForMinutes = true;
		} else if (goForMinutes) {
			std::istringstream(colonHMS.substr(b,n)) >> minutes;
			goForMinutes = false;
			goForSeconds = true;
		} else if (goForSeconds) {
			// Need the goForSets check because the user
			// may accidentally add more data, and we do
			// not want the right data removed! So we do
			// the check and set the variable to false
			std::istringstream(colonHMS.substr(b,n)) >> seconds;
			goForSeconds = false;
		}
		b = colonHMS.find_first_of(digs,n);
		n = colonHMS.find_first_of(sep,b);
	} // End Loop
	
	// Convert time >= 60 mins/secs
	if (seconds >= 60) {
		minutes += (seconds / 60);
		seconds = (seconds % 60);
	}
	
	if (minutes >= 60) {
		hours += (minutes / 60);
		minutes = (minutes % 60);
	}
	
	// Populate the H/M/S string variables
	hoursStr = getPaddedDigitString(hours, 2);
	minutesStr = getPaddedDigitString(minutes, 2);
	secondsStr = getPaddedDigitString(seconds, 2);
	
	// Reuse the colonHMS argument
	colonHMS = (hoursStr + ":" + minutesStr + ":" + secondsStr);
	
	// Return the value
	return colonHMS;
}

bool isValidTime(std::string time) {
	// Initialize the valid time boolean
	bool hoursIsValid = false;
	bool minsIsValid = false;
	bool secsIsValid = false;
	// Initialize the found period size_type
	std::string::size_type periodST = time.find('.');
	
	// If input seconds, find period
	if (periodST != std::string::npos) {
		// By the value and string, determine the
		// validity of seconds, if applicable
		std::string secsStr = time.substr(periodST + 1,
				time.size());
		int secs;
		std::istringstream(secsStr) >> secs;
		if ((secs < 60) && (secs >= 0))
			secsIsValid = true;
	} else
		secsIsValid = true;
	
	// By the value and string, determine the
	// validity of minutes, if applicable
	if (secsIsValid) {
		std::string minsStr;
		int mins;
		// temporarily set minsIsValid to true
		if (periodST != std::string::npos) {
			if (time.substr(0, periodST).size() == 3) {
				minsStr = time.substr(1, 2);
				minsIsValid = true;
			} else if (time.substr(0, periodST).size() == 4) {
				minsStr = time.substr(2, 2);
				minsIsValid = true;
			} else
				minsIsValid = false;
		} else {
			if (time.size() == 3) {
				minsStr = time.substr(1, 2);
				minsIsValid = true;
			} else if (time.size() == 4) {
				minsStr = time.substr(2, 2);
				minsIsValid = true;
			} else
				minsIsValid = false;
		}
		if (minsIsValid) {
			std::istringstream(minsStr) >> mins;
			if ((mins < 60) && (mins >= 0))
				minsIsValid = true;
			else
				minsIsValid = false;
		}
	}
	
	// By the value and string, determine the
	// validity of hours, if applicable
	if (minsIsValid && secsIsValid) {
		std::string hoursStr;
		int hours;
		if (periodST != std::string::npos) {
			if (time.substr(0, periodST).size() == 3) {
				hoursStr = time.substr(0, 1);
				hoursIsValid = true;
			} else if (time.substr(0, periodST).size() == 4) {
				hoursStr = time.substr(0, 2);
				hoursIsValid = true;
			} else
				hoursIsValid = false;
		} else {
			// temporarily set minsIsValid to true
			if (time.size() == 3) {
				hoursStr = time.substr(0, 1);
				hoursIsValid = true;
			} else if (time.size() == 4) {
				hoursStr = time.substr(0, 2);
				hoursIsValid = true;
			} else
				hoursIsValid = false;
		}
		if (hoursIsValid) {
			std::istringstream(hoursStr) >> hours;
			if ((hours < 24) && (hours >= 0))
				hoursIsValid = true;
			else
				hoursIsValid = false;
		}
	}
	
	// Return the value
	return (hoursIsValid && minsIsValid && secsIsValid);
}

std::string getTimePeriod(std::string time) {
	/*
		"Early Morning"
		"Morning"
		"Noon"
		"After Noon"
		"Evening"
		"Night"
		"Late Night"
	*/
	// Declare the time period string
	std::string timePeriod;
	
	// Declare the timeDouble double: time may
	// include seconds (and thus a double)
	double timeDouble;
	std::istringstream(time) >> timeDouble;
	
	// By the value, determine the time period
	if ((timeDouble >= 2300.00) || (timeDouble < 400.00))
		timePeriod = "Late Night";
	else if ((timeDouble >= 400.00) && (timeDouble < 730.00))
		timePeriod = "Early Morning";
	else if ((timeDouble >= 730.00) && (timeDouble < 1100.00))
		timePeriod = "Morning";
	else if ((timeDouble >= 1100.00) && (timeDouble < 1300.00))
		timePeriod = "Noon";
	else if ((timeDouble >= 1300.00) && (timeDouble < 1700.00))
		timePeriod = "After Noon";
	else if ((timeDouble >= 1700.00) && (timeDouble < 2000.00))
		timePeriod = "Evening";
	else if ((timeDouble >= 2000.00) && (timeDouble < 2300.00))
		timePeriod = "Night";
	
	// Return the value
	return timePeriod;
}

/* *********************************************************************
Main Program
 |- Category [Application]: example, "Bodybuilding"
	||- Category [Application] Tasks: example, "Create New Weekly Routine Directory", "Simple Documentation", or "Export to File..."
	
g++ -o PUI.exe PUI.cpp lib/interface/Interface.cpp categories/sporting/bodybuilding/Bodybuilding.cpp categories/sporting/bodybuilding/CreateNewWeeklyDirectory.cpp
********************************************************************* */
/* *********************************************************************
REMOVALS:
--------------
#include "..\ShellControls\ShellControls.h"
#include "..\ShellControls\WindowsClearScreen.h"
#include "..\File\FileIO.h"
	//std::cout << "\nDaysWeeksBeginEnd:: "<<daysWeekBeginEnd;	
********************************************************************* */