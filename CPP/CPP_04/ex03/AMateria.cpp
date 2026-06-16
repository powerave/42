#include "AMateria.hpp"
#include "ICharacter.hpp"

AMateria::AMateria() : _type("Unknown") {
	std::cout << "AMateria Default constructor called" << std::endl;
}

AMateria::AMateria(std::string const &type) : _type(type) {
	std::cout << "AMateria constructor called" << std::endl;
}

AMateria::AMateria(AMateria const &other) : _type(other._type) {
	std::cout << "AMateria copy constructor called" << std::endl;
}

AMateria::~AMateria() {
	std::cout << "AMateria destructor called" << std::endl;
}

void	AMateria::use(ICharacter &name) {
	std::cout << "action on " << name << std::endl;
}

AMateria&	AMateria::operator=(AMateria const &other) {
	if (this != &other)
		this->_type = other._type;
	std::cout << "AMateria copy operator called" <<std::endl;
	return *this;
}

std::string	const &AMateria::getType() const {
	return this->_type;
}

std::ostream& operator<<(std::ostream& os, AMateria const &m) {
	return os << m.getType();
}
