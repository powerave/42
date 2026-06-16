#include "FragTrap.hpp"

int	main(int ac, char **av) {
	if (ac != 2)
		return 1;
	
	int			dmg = 2;
	int			heal = 1;
	ClapTrap	ct("Robot");
	ScavTrap	Scav(av[1]);
	FragTrap	frag("fraggy");
	
	while (ct.getHp() > 0) {
		ct.attack(Scav.getName());
		ct.takeDamage(dmg);
		if (ct.getHp() <= 0)
			break;
		ct.beRepaired(heal);
	}
	if (ct.getHp() <= 0)
		std::cout << ct.getName() << " is all broken <[ x _ o ]>" << std::endl;

	Scav.attack(frag.getName());
	frag.changeHp(Scav.getAd(), 0);
	Scav.guardGate();
	std::cout << frag.getHp() << "Hp <<-- " << frag.getName() << std::endl;
	frag.highFivesGuys();
	return 0;
}
