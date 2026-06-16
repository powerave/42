#include "Zombie.hpp"

int Zombie::_nbZombies = 0;

void Zombie::announce(void)
{
	std::cout << this->_name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

void Zombie::setName(std::string name)
{
	this->_name = name;
}

Zombie*	newZombie(std::string name)
{
	Zombie *Z = new Zombie();
	Z->setName(name);
	return Z;
}

void Zombie::setIndex(int i)
{
	this->_zombieIndex = i;
}

Zombie::~Zombie()
{
	this->_nbZombies -= 1;
	std::cout << this->_name << " has been destroyed" << std::endl;
}