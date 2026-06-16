#pragma once
#include	"Weapon.hpp"
#include	<string>
#include	<iostream>

class	HumanA {
	private:
		Weapon&		_gun;
		std::string	_name;
	public:
		void	attack();
		HumanA(const std::string& name, Weapon& weapon);
		~HumanA();
};
