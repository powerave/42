#include "animal.hpp"
#include "brain.hpp"

class Cat : public Animal {
	private :
		brain* catBrain;
	public:
		Cat();
		~Cat();
		Cat(const Cat &other);
		Cat& operator=(const Cat &other);
		void makeSound() const;
		void setIdea(int index, const std::string &idea);
		std::string getIdea(int index) const;
};
