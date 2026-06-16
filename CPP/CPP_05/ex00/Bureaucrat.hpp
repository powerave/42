#pragma once

#include <iostream>
#include <string>
#include <exception>

#include "GradeTooHighException.hpp"
#include "GradeTooLowException.hpp"

class Bureaucrat {
	private:
		const std::string _name;
		int _grade;
	
	public:
		Bureaucrat();
		Bureaucrat(std::string const& name, int grade);
		Bureaucrat(Bureaucrat const& other);
		~Bureaucrat();
		Bureaucrat& operator=(Bureaucrat const& other);

		const std::string& getName() const;
		int getGrade() const;
		void incrementGrade();
		void decrementGrade();
};

std::ostream& operator<<(std::ostream& os, Bureaucrat const& b);