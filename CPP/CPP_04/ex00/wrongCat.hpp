#include "wrongAnimal.hpp"

class wrongCat : public wrongAnimal {
	public:
		wrongCat();
		~wrongCat();
		wrongCat(const wrongCat &other);
		wrongCat& operator=(const wrongCat &other);
		void makeSound() const;
};