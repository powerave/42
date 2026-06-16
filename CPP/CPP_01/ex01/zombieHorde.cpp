#include "Zombie.hpp"

Zombie*	zombieHorde(int n, std::string name)
{
	int		i = 0;
	Zombie	*Z = new Zombie[n];

	while (i < n)
	{
		Z[i].setName(name);
		Z[i].setIndex(i);
		Z[i].announce();
		i++;
	}
	return Z;
}