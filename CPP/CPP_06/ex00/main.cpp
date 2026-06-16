#include "NotPrintable.hpp"
#include "ScalarConverter.hpp"
#include "InfException.hpp"

int main(int ac, char **av)
{
	if (ac != 2)
		return 0;
	
	try {
		ScalarConverter::convert(av[1]);
	}
	catch (const InfException& e) {
		std::cout << e.what() << std::endl;
		std::cout << "int: infinite value" << std::endl;
		std::cout << "float: infinite value" << std::endl;
		std::cout << "double: infinite value" << std::endl;
		return 0;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: nanf" << std::endl;
		std::cout << "double: nan" << std::endl;
		return 0;
	}
	return 0;
}

// GERER LES FLOAT .0f ET DOUBLE .0