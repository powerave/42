#include "DiamondTrap.hpp"

int	main(int ac, char **av) {
	if (ac != 2)
		return 1;
	
	int			dmg = 2;
	int			heal = 1;
	ClapTrap	ct("Robot");
	FragTrap	frag("fraggy");
	DiamondTrap	diamond("Diamond");
	ScavTrap	Scav(av[1]);

	
	// Test ClapTrap
	std::cout << "=== Testing ClapTrap ===" << std::endl;
	std::cout << "ClapTrap Name: " << ct.getName() << std::endl;
	std::cout << "ClapTrap HP: " << ct.getHp() << std::endl;
	std::cout << "ClapTrap NRG: " << ct.getNrg() << std::endl;
	while (ct.getHp() > 0) {
		ct.attack("Max");
		ct.takeDamage(dmg);
		if (ct.getHp() <= 0)
			break;
		ct.beRepaired(heal);
	}
	if (ct.getHp() <= 0)
		std::cout << ct.getName() << " is all broken <[ x _ o ]>" << std::endl;

	// Test ScavTrap
	std::cout << "\n=== Testing ScavTrap ===" << std::endl;
	std::cout << "ScavTrap Name: " << Scav.getName() << std::endl;
	std::cout << "ScavTrap HP: " << Scav.getHp() << std::endl;
	std::cout << "ScavTrap NRG: " << Scav.getNrg() << std::endl;
	Scav.attack(frag.getName());
	Scav.guardGate();

	// Test FragTrap
	std::cout << "\n=== Testing FragTrap ===" << std::endl;
	std::cout << "FragTrap Name: " << frag.getName() << std::endl;
	std::cout << "FragTrap HP: " << frag.getHp() << std::endl;
	std::cout << "FragTrap NRG: " << frag.getNrg() << std::endl;
	frag.highFivesGuys();

	// Test DiamondTrap
	std::cout << "\n=== Testing DiamondTrap ===" << std::endl;
	std::cout << "DiamondTrap Name: " << diamond.getName() << std::endl;
	std::cout << "DiamondTrap HP: " << diamond.getHp() << std::endl;
	std::cout << "DiamondTrap NRG: " << diamond.getNrg() << std::endl;
	diamond.attack("target");
	diamond.highFivesGuys();
	diamond.whoAmI();

	return 0;
}
