#include "Zombie.hpp"

Zombie	*newZombie(std::string name)
{
	Zombie *Z = new Zombie();
	Z->setName(name);
	return Z;
}
