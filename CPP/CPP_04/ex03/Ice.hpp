#pragma once
#include "AMateria.hpp"

class Ice : public AMateria {
	public :
		Ice();
		Ice(Ice const &other);
		virtual ~Ice();
		Ice& operator=(Ice const &other);
		virtual AMateria* clone() const;
		virtual void use(ICharacter &targer);
};
