#include "Cure.hpp"

Cure::Cure() : AMateria("cure") {
	std::cout << "Cure default constructor called" << std::endl;
}

Cure::~Cure() {
	std::cout << "Cure destructor called" << std::endl;
}

Cure::Cure(Cure const &other) : AMateria(other) {
	std::cout << "Cure copy constructor called" << std::endl;
}

Cure&	Cure::operator=(Cure const &other) {
	(void)other;
	return *this;
}

AMateria* Cure::clone() const {
	return new Cure(*this);
}

void	Cure::use(ICharacter &target) {
	std::cout << "Heals " << target.getName() << "'s wounds" << std::endl;
}