#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

int main() {
	std::cout << "=== Test 1: ShrubberyCreationForm ===" << std::endl;
	try {
		Bureaucrat bob("Bob", 137);
		ShrubberyCreationForm shrubbery("home");
		
		std::cout << bob << std::endl;
		std::cout << shrubbery << std::endl;
		
		bob.signForm(shrubbery);
		bob.executeForm(shrubbery);
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 2: RobotomyRequestForm success/fail ===" << std::endl;
	try {
		Bureaucrat alice("Alice", 45);
		RobotomyRequestForm robotomy("Bender");
		
		alice.signForm(robotomy);
		alice.executeForm(robotomy);
		alice.executeForm(robotomy); // relancer plusieurs fois pour voir les 50%
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 3: PresidentialPardonForm ===" << std::endl;
	try {
		Bureaucrat president("President", 5);
		PresidentialPardonForm pardon("Arthur Dent");
		
		president.signForm(pardon);
		president.executeForm(pardon);
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 4: Execute without signature ===" << std::endl;
	try {
		Bureaucrat boss("Boss", 1);
		PresidentialPardonForm pardon2("Criminal");
		
		boss.executeForm(pardon2);  // Should fail - not signed
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 5: Grade too low to execute ===" << std::endl;
	try {
		Bureaucrat lowGrade("LowGrade", 149);
		ShrubberyCreationForm shrubbery2("garden");
		
		lowGrade.signForm(shrubbery2);  // Can sign (145)
		lowGrade.executeForm(shrubbery2);  // Can't execute (137 needed)
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}
