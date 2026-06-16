#include "ScavTrap.hpp"

ScavTrap::ScavTrap() : ClapTrap() {
	_Hp = 100;
	_Nrg = 50;
	_Ad = 20;
	std::cout << "ScavTrap default constructor called" << std::endl;
}

ScavTrap::ScavTrap(const std::string& name) : ClapTrap(name) {
	_Hp = 100;
	_Nrg = 50;
	_Ad = 20;
	std::cout << "ScavTrap constructor called" << std::endl;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other) {
	std::cout << "ScavTrap copy constructor called" << std::endl;
}

ScavTrap::~ScavTrap() {
	std::cout << "ScavTrap destructor called" << std::endl;
}

void	ScavTrap::guardGate() {
	std::cout << this->getName() << " is now in Gate Keeper mode" << std::endl;
}

void	ScavTrap::attack(const std::string& id) {
	std::cout << this->getName() << " attacked " << id << std::endl;
}

std::string	ScavTrap::getId() const {
	return this->getName();
}
