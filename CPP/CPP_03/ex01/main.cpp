#include "ScavTrap.hpp"

int	main(int ac, char **av) {
	if (ac != 2)
		return 1;
	
	int			dmg = 2;
	int			heal = 1;
	ClapTrap	ct("Robot");
	ScavTrap	Scav(av[1]);

	while (ct.getHp() > 0) {
		ct.attack("MAX");
		ct.takeDamage(dmg);
		if (ct.getHp() <= 0)
			break;
		ct.beRepaired(heal);
	}
	if (ct.getHp() <= 0)
		std::cout << ct.getName() << " is all broken <[ x _ o ]>" << std::endl;

	Scav.attack(ct.getName());
	Scav.guardGate();
	return 0;
}
