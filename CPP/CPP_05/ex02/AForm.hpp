#pragma once

#include <iostream>
#include <string>
#include "GradeTooHighException.hpp"
#include "GradeTooLowException.hpp"

class Bureaucrat;

class AForm {
	private:
		const std::string _name;
		bool _signed;
		const int _signGrade;
		const int _execGrade;
	public:
		AForm();
		AForm(std::string const& name, int sign, int exec);
		virtual ~AForm();
		AForm(AForm const& other);
		AForm& operator=(AForm const& other);
		void beSigned(Bureaucrat const& other);
		std::string getName() const;
		bool isSigned() const;
		int getGrade() const;
		int getExec() const;
		virtual void execute(Bureaucrat const& executor) const = 0;
};

std::ostream& operator<<(std::ostream& os, AForm const& f);
