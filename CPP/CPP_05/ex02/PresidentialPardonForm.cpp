#include "PresidentialPardonForm.hpp"
#include "Bureaucrat.hpp"

PresidentialPardonForm::PresidentialPardonForm() : AForm("Pardon", 25, 5), _target("Unknown") {}

PresidentialPardonForm::PresidentialPardonForm(std::string const& target) : AForm("Pardon", 25, 5), _target(target) {}

PresidentialPardonForm::PresidentialPardonForm(PresidentialPardonForm const& other) : AForm(other), _target(other._target) {}

PresidentialPardonForm& PresidentialPardonForm::operator=(PresidentialPardonForm const& other) {
	if (this != &other)
		this->_target = other.getTarget();
	return *this;
}

PresidentialPardonForm::~PresidentialPardonForm() {}

std::string PresidentialPardonForm::getTarget() const {
	return this->_target;
}

void PresidentialPardonForm::execute(Bureaucrat const& executor) const {
	if (!this->isSigned())
		throw GradeTooLowException();
	if (this->getExec() < executor.getGrade())
		throw GradeTooLowException();
	std::cout << this->getTarget() << " has been pardoned by Zaphod Beeblebrox" << std::endl;
}