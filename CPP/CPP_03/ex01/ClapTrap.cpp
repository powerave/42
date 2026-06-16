#include "ClapTrap.hpp"

ClapTrap::ClapTrap() : _Name("") {
	std::cout << "Default constructor called" << std::endl;
}

ClapTrap::ClapTrap(const std::string Name) : _Name(Name) {
	_Hp = 10;
	_Nrg = 10;
	_Ad = 0;
	std::cout << "Constructor called" << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap& other) : _Name(other._Name), _Hp(other._Hp), _Nrg(other._Nrg), _Ad(other._Ad) {
	std::cout << "Recopy constructor called" << std::endl;
}

ClapTrap::~ClapTrap() {
	std::cout << "Destructor called" << std::endl;
}

ClapTrap& ClapTrap::operator=(const ClapTrap& other) {
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other) {
		_Hp = other._Hp;
		_Nrg = other._Nrg;
		_Ad = other._Ad;
	}
	return *this;
}

int		ClapTrap::changeHp(int hp, bool choice) {
	if (choice == 0)
		return (this->_Hp -= hp);
	return (this->_Hp += hp);
}

void	ClapTrap::attack(const std::string& target) {
	if (this->_Nrg > 0) {
		std::cout << "ClapTrap " << this->_Name << " attacks " << target << " causing " << this->_Ad << " damage" << std::endl;
		this->_Nrg--;
		std::cout << "Energy left: " << this->_Nrg << std::endl;
	}
	else {
		std::cout << "Energy is too low" << std::endl;
		std::cout << "Energy left: " << this->_Nrg << std::endl;
	}
}

void	ClapTrap::takeDamage(unsigned int amount) {
	std::cout << "ClapTrap " << this->_Name << " got hit for " << amount << " damage" << std::endl;
	std::cout << "HP left: " << this->_Hp - (int)amount << std::endl;
	this->changeHp(amount, 0);
}

void	ClapTrap::beRepaired(unsigned int amount) {
	if (this->_Nrg > 0) {
		std::cout << "ClapTrap " << this->_Name << " repaired for " << amount << " HP" <<std::endl;
		this->changeHp(amount, 1);
		this->_Nrg--;
		std::cout << "Energy left: " << this->_Nrg << std::endl;
	}
	else {
		std::cout << "Energy is too low" << std::endl;
		std::cout << "Energy left: " << this->_Nrg << std::endl;		
	}
}

int		ClapTrap::getNrg() const {
	return this->_Nrg;
}

int		ClapTrap::getHp() const {
	return this->_Hp;
}

std::string	ClapTrap::getName() const {
	return _Name;
}

void		ClapTrap::setAd(int newAd) {
	this->_Ad = newAd;
	return ;
}
