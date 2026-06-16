#include "HumanA.hpp"

void	HumanA::attack() {
	std::cout << _name << " attacks with their " << _gun.getType() << std::endl;
}

HumanA::HumanA(const std::string& name, Weapon& weapon) : _gun(weapon), _name(name) {}
HumanA::~HumanA() {}
