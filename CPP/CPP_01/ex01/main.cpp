#include "Zombie.hpp"

int	main()
{
	Zombie* laHorde = zombieHorde(8, "Joe Biden");
	delete[] laHorde;
	return 0;
}