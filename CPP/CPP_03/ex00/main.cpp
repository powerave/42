#include "ClapTrap.hpp"

int	main(int ac, char **av) {
	if (ac != 2)
		return 1;
	
	int			dmg = 4;
	int			heal = 1;
	std::string	name = av[1];
	ClapTrap	ct("Robot");
	
	while (ct.getHp() > 0) {
		ct.attack(name);
		ct.takeDamage(dmg);
		if (ct.getHp() <= 0)
			break;
		ct.beRepaired(heal);
	}
	if (ct.getHp() <= 0)
		std::cout << ct.getName() << " is all broken <[ x _ o ]>" << std::endl;
}
