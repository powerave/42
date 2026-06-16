#pragma once

#include <exception>

class GradeTooHighException : public std::exception {
	public:
		virtual const char* what() const throw();
};
