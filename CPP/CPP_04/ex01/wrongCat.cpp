#include "wrongCat.hpp"

wrongCat::wrongCat() {
	this->type = "wrongCat";
	std::cout << "wrongCat default constructor called" << std::endl;
}

wrongCat::~wrongCat() {
	std::cout << "wrongCat destructor called" << std::endl;
}

wrongCat::wrongCat(const wrongCat &other) : wrongAnimal(other) {
	std::cout << "wrongCat copy constructor called" << std::endl;
}

wrongCat& wrongCat::operator=(const wrongCat &other) {
	if (this != &other)
		wrongAnimal::operator=(other);
	std::cout << "wrongCat copy assignment operator called" << std::endl;
	return *this;
}

void wrongCat::makeSound() const {
	std::cout << "wrongCat makes sound" << std::endl;
}
