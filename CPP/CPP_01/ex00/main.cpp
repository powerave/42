#include "Zombie.hpp"

int main(void)
{
	Zombie *X = newZombie("Bloody");
	X->announce();
	Zombie *Y = newZombie("Mary");
	Y->announce();

	Zombie zombie;
	zombie.setName("Joe Biden");
	zombie.announce();

	randomChump("hola");
	delete X;
	delete Y;
	return (0);
}
