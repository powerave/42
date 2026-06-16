#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap() : FragTrap(), ScavTrap() {
	_Name = "Default_Dia";
	std::cout << "DiamondTrap default constructor called" << std::endl;
}

DiamondTrap::DiamondTrap(std::string name) : ClapTrap(name + "_Dia"), FragTrap(name), ScavTrap(name) {
	_Name = "DIAMOND";
	std::cout << _Name << " constructor called" << std::endl;
	this->_Hp = FragTrap::_Hp;
	this->_Nrg = ScavTrap::_Nrg;
	this->_Ad = FragTrap::_Ad;
}

DiamondTrap::DiamondTrap(const DiamondTrap& other) : ClapTrap(other), FragTrap(other), ScavTrap(other), _Name(other._Name) {
	std::cout << _Name << " copy constructor called" << std::endl;
}

DiamondTrap::~DiamondTrap() {
	std::cout << _Name << " destructor called" << std::endl;
}

void	DiamondTrap::whoAmI() {
	std::cout << "DiamondTrap name: " << this->_Name << ", ClapTrap name: " << this->ClapTrap::_Name << std::endl;
}
