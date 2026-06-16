#include "animal.hpp"

class wrongAnimal {
	protected :
		std::string type;
	public :
		wrongAnimal();
		~wrongAnimal();
		wrongAnimal(const wrongAnimal &other);
		wrongAnimal& operator=(const wrongAnimal &other);
		std::string getType() const;
		void makeSound() const;
};
