#include "Bureaucrat.hpp"
#include "Intern.hpp"
#include <iostream>

int main() {
	std::cout << "=== Test 1: Intern creates valid forms ===" << std::endl;
	try {
		Intern intern;
		Bureaucrat boss("Boss", 1);
		
		AForm* form1 = intern.makeForm("scf", "home");
		AForm* form2 = intern.makeForm("rrf", "Bender");
		AForm* form3 = intern.makeForm("ppf", "Arthur");
		
		if (form1) {
			boss.signForm(*form1);
			boss.executeForm(*form1);
			delete form1;
		}
		
		if (form2) {
			boss.signForm(*form2);
			boss.executeForm(*form2);
			delete form2;
		}
		
		if (form3) {
			boss.signForm(*form3);
			boss.executeForm(*form3);
			delete form3;
		}
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 2: Intern fails to create invalid form ===" << std::endl;
	try {
		Intern intern;
		AForm* form = intern.makeForm("invalid form", "target");
		
		if (form) {
			delete form;
		}
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}
