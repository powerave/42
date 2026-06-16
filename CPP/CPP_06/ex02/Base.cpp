#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <exception>

Base::~Base() {}

Base* generate(void) {
	int random = std::rand() % 3;
	if (random == 0)
		return new A();
	else if (random == 1)
		return new B();
	else
		return new C();
}

void identify(Base* p) {
	if (dynamic_cast<A*>(p) != NULL)
		std::cout << "type: A*" << std::endl;
	else if (dynamic_cast<B*>(p) != NULL)
		std::cout << "type: B*" << std::endl;
	else if (dynamic_cast<C*>(p) != NULL)
		std::cout << "type: C*" << std::endl;
}

void identify(Base& p) {
	try {
		(void)dynamic_cast<A&>(p);
		std::cout << "type: A&" << std::endl;
		return;
	} catch (std::exception&) {}

	try {
		(void)dynamic_cast<B&>(p);
		std::cout << "type: B&" << std::endl;
		return;
	} catch (std::exception&) {}

	try {
		(void)dynamic_cast<C&>(p);
		std::cout << "type: C&" << std::endl;
		return;
	} catch (std::exception&) {}
}
