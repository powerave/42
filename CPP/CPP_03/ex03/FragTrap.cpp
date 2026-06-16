#include "FragTrap.hpp"

FragTrap::FragTrap() : ClapTrap() {
	_Hp = 100;
	_Nrg = 100;
	_Ad = 30;
	std::cout << "FragTrap default constructor called" << std::endl;
}

FragTrap::FragTrap(std::string name) : ClapTrap(name + "_Frag") {
	_Hp = 100;
	_Nrg = 100;
	_Ad = 30;
	std::cout << this->getName() << " constructor called" << std::endl;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other) {
	std::cout << this->getName() << " copy constructor called" << std::endl;
}

FragTrap::~FragTrap() {
	std::cout << this->getName() << " destructor called" << std::endl;
}

void	FragTrap::attack(const std::string& id) {
	std::cout << this->getName() << " attacked " << id << std::endl;
}

void	FragTrap::highFivesGuys() {
	std::cout << "High Fives Guys *o*" << std::endl;
}
