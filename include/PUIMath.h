#ifndef PUIMATH_H
#define PUIMATH_H

bool isDecimal(std::string num);

bool isDecimal(double num);

std::string getNumberWithPreciseDecimalPlaces(const int DECIMAL_PLACES,
		double &num);

#endif