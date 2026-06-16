#include "brain.hpp"

brain::brain() {
	for (int i = 0; i < 100; i++) {
		ideas[i] = "No idea";
	}
	std::cout << "Brain default constructor called" << std::endl;
}

brain::~brain() {
	std::cout << "Brain destructor called" << std::endl;
}

brain::brain(const brain &other) {
	for (int i = 0; i < 100; i++) {
		this->ideas[i] = other.ideas[i];
	}
	std::cout << "Brain copy constructor called" << std::endl;
}

brain& brain::operator=(const brain &other) {
	if (this != &other) {
		for (int i = 0; i < 100; i++) {
			this->ideas[i] = other.ideas[i];
		}
	}
	std::cout << "Brain copy assignment operator called" << std::endl;
	return *this;
}

std::string brain::getIdea(int index) const {
	if (index < 0 || index >= 100) {
		return "Invalid index";
	}
	return ideas[index];
}

void brain::setIdea(int index, const std::string &idea) {
	if (index < 0 || index >= 100) {
		return;
	}
	ideas[index] = idea;
}
