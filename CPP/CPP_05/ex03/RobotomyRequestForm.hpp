#pragma once

#include "AForm.hpp"

class RobotomyRequestForm : public AForm {
	private:
		std::string _target;

	public:
		RobotomyRequestForm();
		RobotomyRequestForm(std::string const& target);
		RobotomyRequestForm(RobotomyRequestForm const& other);
		virtual ~RobotomyRequestForm();
		RobotomyRequestForm& operator=(RobotomyRequestForm const& other);
		std::string getTarget() const;
		virtual void execute(Bureaucrat const& executor) const;
};
