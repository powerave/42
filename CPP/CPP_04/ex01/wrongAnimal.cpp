#include "wrongAnimal.hpp"

wrongAnimal::wrongAnimal() : type("wrongAnimal") {
	std::cout << "wrongAnimal default constructor called" << std::endl;
}

wrongAnimal::~wrongAnimal() {
	std::cout << "wrongAnimal destructor called" << std::endl;
}

wrongAnimal::wrongAnimal(const wrongAnimal &other) : type(other.type) {
	std::cout << "wrongAnimal copy constructor called" << std::endl;
}

wrongAnimal& wrongAnimal::operator=(const wrongAnimal &other) {
	if (this != &other)
		this->type = other.type;
	std::cout << "wrongAnimal copy assignment operator called" << std::endl;
	return *this;
}

std::string wrongAnimal::getType() const {
	return this->type;
}

void wrongAnimal::makeSound() const {
	std::cout << "wrongAnimal makes sound" << std::endl;
}
