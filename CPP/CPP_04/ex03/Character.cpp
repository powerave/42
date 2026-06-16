#include "Character.hpp"
#include "AMateria.hpp"

Character::Character() : _Name("No Name") {
	for (int i = 0; i < 4; i++)
		_Inventory[i] = NULL;
	std::cout << "Character default constructor called" << std::endl;
}

Character::Character(std::string const &name) : _Name(name) {
	for (int i = 0; i < 4; i++)
		_Inventory[i] = NULL;
	std::cout << "Character constructor called" << std::endl;
}

Character::Character(Character const &other) : _Name(other._Name) {
	for(int i = 0; i < 4; i++) {
		if (other._Inventory[i])
			_Inventory[i] = other._Inventory[i]->clone();
		else
			_Inventory[i] = NULL;
	}
	std::cout << "Character copy constructor called" << std::endl;
}

Character::~Character() {
	for(int i = 0; i < 4; i++)
		if(_Inventory[i])
			delete _Inventory[i];
	std::cout << "Character destructor called" << std::endl;
}

Character&	Character::operator=(Character const &other) {
	if (this != &other) {
		_Name = other._Name;
		for(int i = 0; i < 4; i++) {
			if (_Inventory[i])
				delete _Inventory[i];
			if (other._Inventory[i])
				_Inventory[i] = other._Inventory[i]->clone();
			else
				_Inventory[i] = NULL;
		}
	}
	std::cout << "Character asignment operator called" << std::endl;
	return *this;
}

std::string	const &Character::getName() const {
	return _Name;
}

void Character::equip(AMateria* m) {
	if (!m)
		return;
	for(int i = 0; i < 4; i++) {
		if (!_Inventory[i]) {
			_Inventory[i] = m;
			std::cout << _Name << " equiped " << m->getType() << std::endl;
			return;
		}
	}
	std::cout << _Name << "'s inventory is full" << std::endl;
}

void Character::unequip(int idx) {
	if (idx >= 0 && idx < 4 && _Inventory[idx]) {
		std::cout << _Name << " unequiped " << _Inventory[idx]->getType() << std::endl;
		_Inventory[idx] = NULL;
	}
	else
		std::cout << "Nothing to unequip" << std::endl;
}

void Character::use(int idx, ICharacter &target) {
	if (idx >= 0 && idx < 4 && _Inventory[idx]) 
		_Inventory[idx]->use(target);
	else
		std::cout << "Nothing to use" << std::endl;
}	

std::ostream& operator<<(std::ostream& os, Character const &ch) {
	return os << ch.getName();
}