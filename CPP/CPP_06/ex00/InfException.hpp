#pragma once

#include <exception>

class InfException : public std::exception {
	public:
		virtual const char* what() const throw();
};
