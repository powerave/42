#include "Intern.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern() {}

Intern::Intern(Intern const& other) {
	(void)other;
}

Intern::~Intern() {}

Intern& Intern::operator=(Intern const& other) {
	(void)other;
	return *this;
}

AForm* Intern::makeForm(std::string const& name, std::string const& target) {
	if (name == "scf") {
		std::cout << "Intern created " << name << std::endl;
		return new ShrubberyCreationForm(target);
	}
	if (name == "ppf") {
		std::cout << "Intern created " << name << std::endl;
		return new PresidentialPardonForm(target);
	}
	if (name == "rrf") {
		std::cout << "Intern created " << name << std::endl;
		return new RobotomyRequestForm(target);
	}
	std::cout << "Intern couldn't create " << name << " form" << std::endl;
	return NULL;
}
