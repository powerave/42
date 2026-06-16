#include "Base.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main() {
	std::srand(std::time(NULL));

	for (int i = 0; i < 5; i++) {
		Base* p = generate();
		std::cout << "ITERATION No " << i + 1 << std::endl;
		identify(p);
		identify(*p);
		delete p;
		std::cout << "_______________" << std::endl;
	}
	return 0;
}
