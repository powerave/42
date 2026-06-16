#pragma once
#include <string>
#include <iostream>

class	Zombie {
	private:
		std::string _name;
	public:
		void 	announce(void);
		void	setName(std::string name);
		~Zombie(void);
};


Zombie*	newZombie(std::string name);
void	randomChump(std::string name);