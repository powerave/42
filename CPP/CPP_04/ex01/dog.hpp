#include "animal.hpp"
#include "brain.hpp"

class Dog : public Animal {
	private :
		brain* dogBrain;
	public :
		Dog();
		~Dog();
		Dog(const Dog& other);
		Dog& operator=(const Dog& other);
		void makeSound() const;
		void setIdea(int index, const std::string &idea);
		std::string getIdea(int index) const;
};
