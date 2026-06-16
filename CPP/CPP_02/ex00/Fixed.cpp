#include "Fixed.hpp"

Fixed::Fixed() : fixed_int(0) {
	std::cout << "Default constructor called" << std::endl;
}
Fixed::Fixed(const Fixed& other) : fixed_int(other.fixed_int) {
	std::cout << "Copy constructor called" << std::endl;
}
Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}
Fixed&	Fixed::operator=(const Fixed& other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other) {
		fixed_int = other.fixed_int;
	}
	return *this;
}
int		Fixed::getRawBits() const{
	std::cout << "getRawBits member function called" << std::endl;
	return this->fixed_int;
}
void	Fixed::setRawBits(int const raw) {
	this->fixed_int = raw;
}
