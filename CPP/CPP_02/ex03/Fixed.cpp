#include "Fixed.hpp"

Fixed::Fixed() : _pointValue(0) {}

Fixed::Fixed(const int value) : _pointValue(value << this->_fract) {}

Fixed::Fixed(const float value)  {
	this->_pointValue = roundf(value * (1 << this->_fract)); }

Fixed::Fixed(const Fixed& other) : _pointValue(other._pointValue) {}

Fixed::~Fixed() {}

Fixed& Fixed::operator=(const Fixed other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
	{
		this->_pointValue = other.getRawBits();
	}
	return *this;
}

int		Fixed::getRawBits() const {
	return	this->_pointValue;
}

void	Fixed::setRawBits(const int raw) {
	this->_pointValue = raw;
}

float	Fixed::toFloat() const {
	return	(static_cast<float>(this->_pointValue) / (1 << this->_fract)); 
}

int		Fixed::toInt() const {
	return	(this->_pointValue >> this->_fract);
}

bool	Fixed::operator<(const Fixed& other) const {
	return (this->_pointValue < other._pointValue);
}
bool	Fixed::operator>(const Fixed& other) const {
	return (this->_pointValue > other._pointValue);
}
bool	Fixed::operator<=(const Fixed& other) const {
	return (this->_pointValue <= other._pointValue);
}
bool	Fixed::operator>=(const Fixed& other) const {
	return (this->_pointValue >= other._pointValue);
}
bool	Fixed::operator!=(const Fixed& other) const {
	return (this->_pointValue != other._pointValue);
}
bool	Fixed::operator==(const Fixed& other) const {
	return (this->_pointValue == other._pointValue);
}

Fixed	Fixed::operator++(int) {
	Fixed	tmp;

	tmp = *this;
	this->_pointValue += (1 << this->_fract);
	return tmp;
}

Fixed&	Fixed::operator++() {
	this->_pointValue += (1 << this->_fract);
	return *this;
}

Fixed	Fixed::operator--(int) {
	Fixed	tmp;

	tmp = *this;
	this->_pointValue -= (1 << this->_fract);
	return tmp;
}

Fixed&	Fixed::operator--() {
	this->_pointValue -= (1 << this->_fract);
	return *this;
}

Fixed	Fixed::operator+(const Fixed& other) const {
	Fixed	result;

	result._pointValue = this->_pointValue + other._pointValue;
	return result;
}

Fixed	Fixed::operator-(const Fixed& other) const {
	Fixed	result;

	result._pointValue = this->_pointValue - other._pointValue;
	return result;
}

Fixed	Fixed::operator*(const Fixed& other) const {
	Fixed	result;

	result._pointValue = static_cast<int>((static_cast<long long>(this->_pointValue) * other._pointValue) >> _fract);
	return result; 
}

Fixed	Fixed::operator/(const Fixed& other) const {
	Fixed	result;

	result._pointValue = static_cast<int>((static_cast<long long>(this->_pointValue) << _fract) / other._pointValue);
	return result; 
}

Fixed	Fixed::min(Fixed& a, Fixed& b) {
	if (a._pointValue < b._pointValue)
		return a;
	else
		return b;
}

Fixed	Fixed::min(const Fixed& a, const Fixed& b) {
	if (a._pointValue < b._pointValue)
		return a;
	else
		return b;
}

Fixed	Fixed::max(Fixed& a, Fixed& b) {
	if (a._pointValue > b._pointValue)
		return a;
	else
		return b;
}

Fixed	Fixed::max(const Fixed&a, const Fixed& b) {
	if (a._pointValue > b._pointValue)
		return a;
	else
		return b;
}

std::ostream&	operator<<(std::ostream& out, const Fixed& fixed) {
	out << fixed.toFloat();
	return out;
}