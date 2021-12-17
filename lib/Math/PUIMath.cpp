// Regular includes
#include <iomanip>
#include <sstream>
#include <cmath>
// Application includes
#include "..\..\include\PUIMath.h"

bool isDecimal(std::string num) {
	// Return the value
	return (num.find(".") != std::string::npos);
}

bool isDecimal(double num) {
	// Convert the number to a string
	std::stringstream numStream;
	numStream << num;
	std::string numStr = numStream.str();
	
	// Return the value
	return isDecimal(numStr);
}

std::string getNumberWithPreciseDecimalPlaces(const int DECIMAL_PLACES,
		double &num) {
	std::string returnVal;
	// Initialize a string STREAM to get the number as a string
	std::stringstream numStream;
	numStream << std::setprecision(16) << num;
	
	// Initialize a string to hold the streamed number
	std::string numInStr = numStream.str();
	
	// Declare a temporary variable to hold the number before
	// the decimal
	std::string tmpBefore;
	
	// Declare a temporary variable to hold the number after
	// the decimal
	std::string tmpAfter;
	
	// Check if a decimal is present...if one is, then assign
	// the data before the decimal to the tmp string
	if ((isDecimal(numInStr)) && (DECIMAL_PLACES != 0)) {
		// Get the place of the decimal
		std::string::size_type decimalPos = numInStr.find(".");
		
		tmpBefore = numInStr.substr(0, decimalPos);
		double decimalNumber;
		// Stream the number from the decimal point to the end
		std::istringstream(numInStr.substr(decimalPos,
				numInStr.size())) >> decimalNumber;
		// Stream the decimal portion to a string with precision
		// set at the desired decimal places
		std::stringstream decStream;
		decStream << std::setprecision(DECIMAL_PLACES) << decimalNumber;
		// Declare a temporary string for the conversion to string
		// so we can get the decimal and numbers after it to assign
		// to tmpAfter and concatenate the data with tmpBefore to
		// convert the two to a number and return it
		std::string decNumStr = decStream.str();
		// Since 0's are prepended to doubles, we need to get the
		// position of the decimal (in case one or more zeros were
		// prepended) and the rest of the decimal number
		tmpAfter = decNumStr.substr(decNumStr.find('.'), decNumStr.size());
		// Because decimals are not a guarantee, we need to reuse
		// numInStr by concatenating the before and after decimal
		// tmp strings to istringstream the number string to a number
		numInStr = (tmpBefore + tmpAfter);
		
		// Set the numeric value to the number
		std::istringstream(numInStr) >> num;
		
		// Convert the new number back to a string with precision
		// as the length of the string!
		std::stringstream outStream;
		outStream << std::setprecision(numInStr.size()) << num;
		returnVal = outStream.str();
	} else if (!isDecimal(numInStr)) {
		// Convert the number to a string and return it at
		// it's exact value! (conversion to string already
		// accomplished, so assign the return value to the
		// number in string format to return)
		returnVal = numInStr;
	} else {
		// Convert the number to the rounded (up or down as
		// necessary) numerical string and return it
		int topVal = ceil(num);
		if ((topVal - num) <= .5) {
			std::stringstream outStream;
			outStream << topVal;
			returnVal = outStream.str();
		} else {
			std::stringstream outStream;
			outStream << floor(num);
			returnVal = outStream.str();
		}
	}
	
	// Return the value
	return returnVal;
}
