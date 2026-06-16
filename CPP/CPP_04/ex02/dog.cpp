#include "dog.hpp"
#include "brain.hpp"

Dog::Dog() {
	this->_type = "Dog";
	this->dogBrain = new brain();
	std::cout << "Dog default constructor called" << std::endl;
}

Dog::~Dog() {
	std::cout << "Dog destructor called" << std::endl;
	delete dogBrain;
}

Dog::Dog(const Dog& other) : Animal(other) {
	if (other.dogBrain)
		this->dogBrain = new brain(*other.dogBrain);
	else
		this->dogBrain = new brain();
	std::cout << "Dog copy constructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& other) {
	if (this != &other) {
		Animal::operator=(other);
		if (other.dogBrain) {
			if (!this->dogBrain)
				this->dogBrain = new brain(*other.dogBrain);
			else
				*(this->dogBrain) = *(other.dogBrain); // deep copy via operator=
		} else {
			if (this->dogBrain) {
				delete this->dogBrain;
				this->dogBrain = 0;
			}
		}
	}
	std::cout << "Dog copy assignment operator called" << std::endl;
	return *this;
}

void Dog::makeSound() const {
	std::cout << "Woof Woof!" << std::endl;
}

void Dog::setIdea(int index, const std::string &idea) {
	if (dogBrain)
		dogBrain->setIdea(index, idea);
}

std::string Dog::getIdea(int index) const {
	if (dogBrain)
		return dogBrain->getIdea(index);
	return "";
}
