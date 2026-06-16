#include "Bureaucrat.hpp"
#include "Form.hpp"
#include <iostream>

int main() {
	std::cout << "=== Test 1: Create valid forms ===" << std::endl;
	try {
		Form f1("Tax Form", 50, 25);
		std::cout << f1 << std::endl;
		
		Form f2("Building Permit", 100, 75);
		std::cout << f2 << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 2: Form with grade too high ===" << std::endl;
	try {
		Form f3("Invalid Form", 0, 50);
		std::cout << f3 << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 3: Form with grade too low ===" << std::endl;
	try {
		Form f4("Invalid Form", 151, 100);
		std::cout << f4 << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 4: Bureaucrat signs form successfully ===" << std::endl;
	try {
		Bureaucrat bob("Bob", 30);
		Form f5("Contract", 50, 25);
		
		std::cout << bob << std::endl;
		
		bob.signForm(f5);
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 5: Bureaucrat cannot sign (grade too low) ===" << std::endl;
	try {
		Bureaucrat john("John", 100);
		Form f6("Secret Document", 50, 25);
		
		std::cout << john << std::endl;
		std::cout << f6 << std::endl;
		
		john.signForm(f6);
		std::cout << f6 << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	std::cout << "\n=== Test 6: Multiple sign attempts ===" << std::endl;
	try {
		Bureaucrat alice("Alice", 1);
		Bureaucrat charlie("Charlie", 150);
		Form f7("Important Form", 50, 25);
		
		std::cout << alice << std::endl;
		std::cout << charlie << std::endl;
		std::cout << f7 << std::endl;
		
		charlie.signForm(f7);  // Should fail
		alice.signForm(f7);    // Should succeed
		alice.signForm(f7);    // Already signed
	}
	catch (std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}

	return 0;
}
