#include "animal.hpp"
#include "dog.hpp"
#include "cat.hpp"
#include "wrongCat.hpp"

int main()
{
	std::cout << "=== CONSTRUCTION TABLEAU (Dogs/Cats) ===" << std::endl;
	const int SIZE = 10; // moitié dogs moitié cats
	Animal* animals[SIZE];
	for (int i = 0; i < SIZE; i++) {
		if (i < SIZE / 2)
			animals[i] = new Dog();
		else
			animals[i] = new Cat();
	}

	std::cout << "\n=== TEST POLYMORPHISME (makeSound) ===" << std::endl;
	for (int i = 0; i < SIZE; i++) {
		std::cout << i << " (" << animals[i]->getType() << ") -> ";
		animals[i]->makeSound();
	}

	std::cout << "\n=== TEST COPIE PROFONDE (Dog) ===" << std::endl;
	Dog* dogA = static_cast<Dog*>(animals[0]);
	dogA->setIdea(0, "Chase cats");
	dogA->setIdea(1, "Bury bone");
	Dog dogB(*dogA); // Copy constructor deep
	std::cout << "dogA idea0: " << dogA->getIdea(0) << std::endl;
	std::cout << "dogB idea0: " << dogB.getIdea(0) << std::endl;
	dogA->setIdea(0, "Sleep");
	std::cout << "After change dogA idea0: " << dogA->getIdea(0) << std::endl;
	std::cout << "dogB idea0 (should stay 'Chase cats'): " << dogB.getIdea(0) << std::endl;

	std::cout << "\n=== TEST ASSIGNATION (Dog) ===" << std::endl;
	Dog dogC;
	dogC = *dogA; // assignment deep copy
	dogA->setIdea(1, "Eat");
	std::cout << "dogA idea1: " << dogA->getIdea(1) << std::endl;
	std::cout << "dogC idea1 (should stay 'Bury bone'): " << dogC.getIdea(1) << std::endl;

	std::cout << "\n=== TEST COPIE PROFONDE (Cat) ===" << std::endl;
	Cat* catA = static_cast<Cat*>(animals[SIZE/2]);
	catA->setIdea(0, "Climb" );
	Cat catB(*catA);
	catA->setIdea(0, "Scratch" );
	std::cout << "catA idea0: " << catA->getIdea(0) << std::endl;
	std::cout << "catB idea0 (should stay 'Climb'): " << catB.getIdea(0) << std::endl;

	std::cout << "\n=== TEST ASSIGNATION (Cat) ===" << std::endl;
	Cat catC;
	catC = *catA;
	catA->setIdea(0, "Jump" );
	std::cout << "catA idea0: " << catA->getIdea(0) << std::endl;
	std::cout << "catC idea0 (should stay 'Scratch'): " << catC.getIdea(0) << std::endl;

	std::cout << "\n=== DESTRUCTION TABLEAU (delete via base) ===" << std::endl;
	for (int i = 0; i < SIZE; i++)
		delete animals[i];

	std::cout << "\n=== TEST WRONG (absence de virtual) ===" << std::endl;
	const wrongAnimal* w = new wrongCat();
	w->makeSound(); // n'appelle pas wrongCat
	delete w;

	std::cout << "\n=== FIN TESTS ===" << std::endl;
	return 0;
}