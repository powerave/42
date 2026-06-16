#pragma once

#include <exception>

class NanException : public std::exception {
	public:
		const char* what() const throw();
};
