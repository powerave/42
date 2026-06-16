#pragma once
#include <iostream>
#include <string>

class	ClapTrap {
	protected :
		const std::string		_Name;
		int						_Hp;
		int						_Nrg;
		int						_Ad;

	public :
		ClapTrap();
		ClapTrap(const std::string Name);
		ClapTrap(const ClapTrap& other);
		~ClapTrap();
		ClapTrap& operator=(const ClapTrap& other);
		void		attack(const std::string& target);
		void		takeDamage(unsigned int amount);
		void		beRepaired(unsigned int amount);
		int			changeHp(int hp, bool choice);
		int			getHp() const;
		int			getNrg() const;
		std::string	getName() const;
		void		setAd(int newAd);
};
