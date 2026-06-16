#pragma once

#include <exception>

class ConvertionException : public std::exception {
	public:
		const char* what() const throw();
};
