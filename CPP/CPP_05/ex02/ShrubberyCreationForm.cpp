#include "ShrubberyCreationForm.hpp"
#include "Bureaucrat.hpp"
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm() : AForm("Shrubbery", 145, 137), _target("Undefined") {}

ShrubberyCreationForm::ShrubberyCreationForm(std::string const& target) : AForm("Shrubbery", 145, 137), _target(target) {}

ShrubberyCreationForm::ShrubberyCreationForm(ShrubberyCreationForm const& other) : AForm(other), _target(other._target) {}

ShrubberyCreationForm::~ShrubberyCreationForm() {}

std::string ShrubberyCreationForm::getTarget() const {
	return this->_target;
}

ShrubberyCreationForm& ShrubberyCreationForm::operator=(ShrubberyCreationForm const& other) {
	if (this != &other) {
		this->_target = other.getTarget();
	}
	return *this;
}


void ShrubberyCreationForm::execute(Bureaucrat const& executor) const {
	if (!this->isSigned())
		throw GradeTooLowException();

	if (executor.getGrade() > this->getExec())
		throw GradeTooLowException();

	std::ofstream file((this->_target + "_shrubbery").c_str());
	
	file << "       ###" << std::endl;
    file << "      #o###" << std::endl;
    file << "    #####o###" << std::endl;
    file << "   #o###|#/###" << std::endl;
    file << "    ####|/#o#" << std::endl;
    file << "     # }|{ #" << std::endl;
    file << "       }|{" << std::endl;
    file << "       }|{" << std::endl;
    file << "       }|{" << std::endl;
    file << "       }|{" << std::endl;

	file.close();
}
