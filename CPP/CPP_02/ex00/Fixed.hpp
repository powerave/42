#pragma once
#include <iostream>
#include <string>

class Fixed {
	private :
		int					fixed_int;
		static const 	int	fract = 8;
		
	public :
		Fixed();
		Fixed(const Fixed& other);
		~Fixed();
		Fixed&	operator=(const Fixed& other);
		int 	getRawBits() const;
		void	setRawBits(int const raw);
};
