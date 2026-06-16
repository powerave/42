#pragma once
#include <iostream>
#include <cmath>

class Fixed {
	private :
		int					_fixedInt;
		static const 	int	_fract = 8;
		
	public :
		Fixed();
		Fixed(const int value);
		Fixed(const float value);
		Fixed(const Fixed& other);
		~Fixed();
		Fixed&	operator=(const Fixed& other);
		int 	getRawBits() const;
		void	setRawBits(int const raw);
		float	toFloat() const;
		int		toInt() const;
};

std::ostream&	operator<<(std::ostream& out, const Fixed& fixed);
