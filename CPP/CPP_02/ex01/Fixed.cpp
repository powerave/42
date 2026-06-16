#include "Fixed.hpp"

Fixed::Fixed() : _fixedInt(0) {
	std::cout << "Default constructor called" << std::endl;
}
Fixed::Fixed(const int value) {
	std::cout << "Int constructor called" << std::endl;
	this->_fixedInt = value << this->_fract;
}
Fixed::Fixed(const Fixed& other) : _fixedInt(other.getRawBits()){
	std::cout << "Copy constructor called" << std::endl;
}
Fixed::~Fixed() {
	std::cout << "Destructor called" << std::endl;
}
Fixed::Fixed(const float value) {
	std::cout << "Float constructor called" << std::endl;
	this->_fixedInt = roundf(value * (1 << this->_fract));
}

Fixed&	Fixed::operator=(const Fixed& other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other) {
		this->_fixedInt = other.getRawBits();
	}
	return *this;
}

int		Fixed::getRawBits() const{
	return (this->_fixedInt);
}
void	Fixed::setRawBits(int const raw) {
	this->_fixedInt = raw;
}

float	Fixed::toFloat() const {
	return (static_cast<float>(this->_fixedInt) / static_cast<float>((1 << this->_fract)));
}

int		Fixed::toInt() const {
	return (this->_fixedInt >> this->_fract);
}

std::ostream&	operator<<(std::ostream& out, const Fixed& fixed) {
	out << fixed.toFloat();
	return out;
}
