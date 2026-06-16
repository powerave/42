#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <exception>
#include <iomanip>
#include "NotPrintable.hpp"
#include "InfException.hpp"
#include "ConvertionException.hpp"
#include "NanException.hpp"

class InfException;
class NotPrintable;
class ConvertionException;

class ScalarConverter {
	private:
		ScalarConverter();
		ScalarConverter(ScalarConverter const& other);
		~ScalarConverter();
		ScalarConverter& operator=(ScalarConverter const& other);
	public:
		static void convert(std::string const& literal);
};
