#include "RPN.hpp"

int main (int ac, char **av) {
	if (ac != 2) {
		std::cout << "Error: Invalid number of arguments." << std::endl;
		return 1;
	}
	RPN rpn(av[1]);
	try {
		rpn.calculate(av[1]);
	} catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
