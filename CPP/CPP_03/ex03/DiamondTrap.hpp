#include "FragTrap.hpp"

class DiamondTrap : public FragTrap, public ScavTrap {
	private :
		std::string		_Name;
	public :
		DiamondTrap();
		DiamondTrap(std::string name);
		DiamondTrap(const DiamondTrap& other);
		~DiamondTrap();
		using ScavTrap::attack;
		void	whoAmI();
};
