#include "Zombie.hpp"

void	randomChump(std::string name)
{
	Zombie *X = newZombie(name);
	X->announce();
	delete X;
}