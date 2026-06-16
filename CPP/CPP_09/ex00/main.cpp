#include "BitcoinExchange.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Error: No file provided." << std::endl;
		return 1;
	}

	try {
		BitcoinExchange btc;
		btc.processFile(av[1]);
	}
	catch (const std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}