#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap() {
	_Hp = 100;
	_Nrg = 50;
	_Ad = 20;
	std::cout << "ScavTrap default constructor called" << std::endl;
}

ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name + "_Scav") {
	_Hp = 100;
	_Nrg = 50;
	_Ad = 20;
	std::cout << _Name << " constructor called" << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
	std::cout << _Name << " copy constructor called" << std::endl;
}

ScavTrap::~ScavTrap() {
	std::cout << _Name << " destructor called" << std::endl;
}

void	ScavTrap::guardGate() {
	std::cout << _Name << " is now in Gate Keeper mode" << std::endl;
}

void	ScavTrap::attack(const std::string& id) {
	std::cout << _Name << " attacked " << id << std::endl;
}
