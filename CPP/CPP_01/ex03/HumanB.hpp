#pragma once
#include	"Weapon.hpp"
#include	<string>
#include	<iostream>

class	HumanB {
	private:
		Weapon*		_gun;
		std::string	_name;
	public:
		void	attack();
		void	setWeapon(Weapon& weapon);
		HumanB(const std::string& name);
		~HumanB();
};
