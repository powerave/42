#include "HumanB.hpp"

void	HumanB::setWeapon(Weapon& weapon) {
	_gun = &weapon;
}

void	HumanB::attack() {
	if (_gun != NULL)
		std::cout << _name << " attacks with their " << _gun->getType() << std::endl;
}

HumanB::HumanB(const std::string& name) : _gun(NULL), _name(name) {}
HumanB::~HumanB() {}