#include "Fixed.hpp"

Fixed::Fixed() : _fixedPointValue(0) {
}

Fixed::Fixed(const int value) : _fixedPointValue(value << this->_fractionalBits) {
}
Fixed::Fixed(const float value) {
	this->_fixedPointValue = roundf(value * (1 << this->_fractionalBits));
}
Fixed::Fixed(const Fixed& other) : _fixedPointValue(other._fixedPointValue) {
}
Fixed::~Fixed() {
}

Fixed& Fixed::operator=(const Fixed other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
	{
		this->_fixedPointValue = other.getRawBits();
	}
	return *this;
}

int		Fixed::getRawBits() const {
	return this->_fixedPointValue;
}
void	Fixed::setRawBits(const int raw) {
	this->_fixedPointValue = raw;
}

float	Fixed::toFloat() const {
	return (static_cast<float>(this->_fixedPointValue) / (1 << this->_fractionalBits));
}
int		Fixed::toInt() const {
	return (this->_fixedPointValue >> this->_fractionalBits);
}

/****************  Standards operators ***************/

Fixed	Fixed::operator+(const Fixed& other){
	Fixed	result;
	result._fixedPointValue = this->_fixedPointValue + other._fixedPointValue;
	return result;
}
Fixed	Fixed::operator-(const Fixed& other) {
	Fixed	result;
	result._fixedPointValue = this->_fixedPointValue - other._fixedPointValue;
	return result;
}
Fixed	Fixed::operator*(const Fixed& other) {
	Fixed	result;
	result._fixedPointValue =  static_cast<int>((static_cast<long long>(this->_fixedPointValue) * other._fixedPointValue) >> _fractionalBits);
	return result;
}
Fixed	Fixed::operator/(const Fixed& other) {
	Fixed	result;
	if (other._fixedPointValue == 0) {
		std::cerr << "error: division par 0\nle resultat a ete mis a 0 par defaut" << std::endl;
		result._fixedPointValue = 0;
		return (result);
	}
	result._fixedPointValue = static_cast<int>((static_cast<long long>(this->_fixedPointValue) << _fractionalBits) / other._fixedPointValue);
	return result;
}

/*************** bool operators ***************/

bool	Fixed::operator<(const Fixed& other) {
	return (this->_fixedPointValue < other._fixedPointValue);
}

bool	Fixed::operator>(const Fixed& other) {
	return (this->_fixedPointValue > other._fixedPointValue);
}

bool	Fixed::operator<=(const Fixed& other) {
	return (this->_fixedPointValue <= other._fixedPointValue);
}

bool	Fixed::operator>=(const Fixed& other) {
	return (this->_fixedPointValue >= other._fixedPointValue);
}

bool	Fixed::operator==(const Fixed& other) {
	return (this->_fixedPointValue == other._fixedPointValue);
}

bool	Fixed::operator!=(const Fixed& other) {
	return (this->_fixedPointValue != other._fixedPointValue);
}

/*************** incrementation operators ***************/

Fixed&	Fixed::operator++() {
	_fixedPointValue++;
	return *this;
}

Fixed&	Fixed::operator--() {
	_fixedPointValue--;
	return *this;
}

Fixed	Fixed::operator++(int) {
	Fixed tmp = *this;
	_fixedPointValue++;
	return tmp;
}

Fixed	Fixed::operator--(int) {
	Fixed tmp = *this;
	_fixedPointValue--;
	return tmp;
}

Fixed	Fixed::min(Fixed& a, Fixed& b) {
	Fixed	tmp;
	if (a._fixedPointValue >= b._fixedPointValue)
		return b;
	else
		return a;
}

Fixed	Fixed::min(const Fixed& a, const Fixed& b) {
	if (a._fixedPointValue >= b._fixedPointValue)
		return b;
	else
		return a;
}

Fixed	Fixed::max(Fixed& a, Fixed& b) {
	if (a._fixedPointValue <= b._fixedPointValue)
		return b;
	else
		return a;
}

Fixed	Fixed::max(const Fixed& a, const Fixed& b) {
	if (a._fixedPointValue <= b._fixedPointValue)
		return b;
	else
		return a;
}

std::ostream&	operator<<(std::ostream& out, const Fixed& fixed) {
	out << fixed.toFloat();
	return out;
}
