#pragma once

#include <exception>

class NotPrintable : public std::exception {
	public:
		virtual const char* what() const throw(); 
};
