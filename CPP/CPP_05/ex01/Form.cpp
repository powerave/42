#include "Form.hpp"

Form::Form() : _name("Unknown"), _signed(false) ,_signGrade(150) , _execGrade(150) {
	std::cout << "default useless form constructed" << std::endl;
}

Form::Form(std::string const& name, int sign, int exec) : _name(name), _signed(false), _signGrade(sign), _execGrade(exec) {
	if (sign < 1 || exec < 1)
		throw GradeTooHighException();
	if (sign > 150 || exec > 150)
		throw GradeTooLowException();
}

Form::Form(Form const& other) : _signed(other._signed), _signGrade(other._signGrade), _execGrade(other._execGrade) {}

Form::~Form() {}

Form& Form::operator=(Form const& other) {
	if (this != &other)
	{
		this->_signed = other._signed;
	}
	return *this;
}

void Form::beSigned(Bureaucrat const& other) {
	if (other.getGrade() > this->_signGrade) {
		this->_signed = false;
		throw GradeTooLowException();
	}
	this->_signed = true;
}

std::string Form::getName() const {
	return this->_name;
}

std::ostream& operator<<(std::ostream& os, Form const& f) {
	os << f.getName();
	return os;
}