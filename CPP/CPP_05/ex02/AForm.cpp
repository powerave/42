#include "AForm.hpp"
#include "Bureaucrat.hpp"

AForm::AForm() : _name("Unknown"), _signed(false) ,_signGrade(150) , _execGrade(150) {
	std::cout << "default useless form constructed" << std::endl;
}

AForm::AForm(std::string const& name, int sign, int exec) : _name(name), _signed(false), _signGrade(sign), _execGrade(exec) {
	if (sign < 1 || exec < 1)
		throw GradeTooHighException();
	if (sign > 150 || exec > 150)
		throw GradeTooLowException();
}

AForm::AForm(AForm const& other) : _signed(other._signed), _signGrade(other._signGrade), _execGrade(other._execGrade) {}

AForm::~AForm() {}

AForm& AForm::operator=(AForm const& other) {
	if (this != &other)
	{
		this->_signed = other._signed;
	}
	return *this;
}

void AForm::beSigned(Bureaucrat const& other) {
	if (other.getGrade() > this->_signGrade) {
		this->_signed = false;
		throw GradeTooLowException();
	}
	this->_signed = true;
}

std::string AForm::getName() const {
	return this->_name;
}

int AForm::getGrade() const {
	return this->_signGrade;
}

int AForm::getExec() const {
	return this->_execGrade;
}

bool AForm::isSigned() const {
	return _signed;
}

std::ostream& operator<<(std::ostream& os, AForm const& f) {
	os << f.getName();
	return os;
}