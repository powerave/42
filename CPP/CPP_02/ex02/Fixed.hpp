#pragma once
#include <iostream>
#include <cmath>

class Fixed {
	private :
		int					_fixedPointValue;
		static const int	_fractionalBits = 8;
	public :
	Fixed();
	Fixed(const int value);
	Fixed(const float value);
	Fixed(const Fixed& other);
	~Fixed();
	Fixed& operator=(const Fixed other);
	int		getRawBits() const;
	void	setRawBits(const int raw);
	float	toFloat() const;
	int		toInt() const;	
 
	static Fixed min(Fixed& a, Fixed& b);
	static Fixed min(const Fixed& a, const Fixed& b);
	static Fixed max(Fixed& a, Fixed& b);
	static Fixed max(const Fixed& a, const Fixed&b);

	bool operator<(const Fixed& other);
	bool operator>(const Fixed& other);
	bool operator<=(const Fixed& other);
	bool operator>=(const Fixed& other);
	bool operator==(const Fixed& other);
	bool operator!=(const Fixed& other);

	Fixed operator+(const Fixed& other);
	Fixed operator-(const Fixed& other);
	Fixed operator*(const Fixed& other);
	Fixed operator/(const Fixed& other);

	Fixed&	operator++();
	Fixed&	operator--();
	Fixed	operator++(int);
	Fixed	operator--(int);
};

std::ostream&	operator<<(std::ostream& out, const Fixed& fixed);
