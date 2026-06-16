#include "RobotomyRequestForm.hpp"
#include "Bureaucrat.hpp"
#include <cstdlib>
#include <ctime>

RobotomyRequestForm::RobotomyRequestForm() : AForm("Robotomy", 72, 45), _target("Undefined") {
	srand(time(NULL));
}

RobotomyRequestForm::RobotomyRequestForm(std::string const& target) : AForm("Robotomy", 72, 45), _target(target) {
	srand(time(NULL));
}

RobotomyRequestForm::RobotomyRequestForm(RobotomyRequestForm const& other) : AForm(other), _target(other._target) {}

RobotomyRequestForm::~RobotomyRequestForm() {}

RobotomyRequestForm& RobotomyRequestForm::operator=(RobotomyRequestForm const& other) {
	if (this != &other) {
		this->_target = other.getTarget();
	}
	return *this;
}

std::string RobotomyRequestForm::getTarget() const {
	return this->_target;
}

void RobotomyRequestForm::execute(Bureaucrat const& executor) const {
	if (!this->isSigned())
	throw GradeTooLowException();
	if (executor.getGrade() > this->getExec())
	throw GradeTooLowException();
	std::cout << this->getName() << " makes some drilling noises" << std::endl;
	if ((rand() % 2) == 0)
		std::cout << this->_target << " has been succesfully robomotized" << std::endl;
	else
		std::cout << "Robotomy failed" << std::endl;	
}
