#pragma once
#include "ICharacter.hpp"

class Character : public ICharacter {
	private :
		std::string _Name;
		AMateria* _Inventory[4];

	public :
		Character();
		Character(std::string const &name);
		~Character();
		Character(Character const &other);
		Character& operator=(Character const &other);
		virtual std::string const &getName() const;
		virtual void equip(AMateria* m);
		virtual void unequip(int idx);
		virtual void use(int idx, ICharacter &target);
};

std::ostream& operator<<(std::ostream& os, Character const &ch);
