#pragma once

#include "AForm.hpp"

class ShrubberyCreationForm : public AForm {
	private:
		std::string _target;
	public:
		ShrubberyCreationForm();
		ShrubberyCreationForm(std::string const& target);
		ShrubberyCreationForm(ShrubberyCreationForm const& other);
		virtual ~ShrubberyCreationForm();
		ShrubberyCreationForm& operator=(ShrubberyCreationForm const& other);
		virtual void execute(Bureaucrat const& executor) const;
		std::string getTarget() const;
};
