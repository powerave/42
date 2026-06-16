// Cat implementation with deep copy of brain
#include "cat.hpp"

Cat::Cat() {
	this->type = "cat";
	this->catBrain = new brain();
	std::cout << "Cat default constructor called" << std::endl;
}

Cat::~Cat() {
	std::cout << "Cat destructor called" << std::endl;
	delete catBrain;
}

Cat::Cat(const Cat &other) : Animal(other) {
	if (other.catBrain)
		this->catBrain = new brain(*other.catBrain);
	else
		this->catBrain = new brain();
	std::cout << "Cat copy constructor called" << std::endl;
}

Cat& Cat::operator=(const Cat &other) {
	if (this != &other) {
		Animal::operator=(other);
		if (other.catBrain) {
			if (!this->catBrain)
				this->catBrain = new brain(*other.catBrain);
			else
				*(this->catBrain) = *(other.catBrain);
		} else {
			if (this->catBrain) {
				delete this->catBrain;
				this->catBrain = 0;
			}
		}
	}
	std::cout << "Cat copy assignment operator called" << std::endl;
	return *this;
}

void Cat::makeSound() const {
	std::cout << "Miaou Miaou!" << std::endl;
}

void Cat::setIdea(int index, const std::string &idea) {
	if (catBrain)
		catBrain->setIdea(index, idea);
}

std::string Cat::getIdea(int index) const {
	if (catBrain)
		return catBrain->getIdea(index);
	return "";
}