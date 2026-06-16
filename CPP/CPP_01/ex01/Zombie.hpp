#pragma once
#include <string>
#include <iostream>

class Zombie {
	
	private:
		int			_zombieIndex;
		static int	_nbZombies;
		std::string _name;
	
	public:
		void	announce(void);
		void	setName(std::string name);
		void	setIndex(int i);
		~Zombie(void);
};

Zombie*	newZombie(std::string name);
Zombie*	zombieHorde(int n, std::string name);
