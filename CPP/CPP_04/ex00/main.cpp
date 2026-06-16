#include "animal.hpp"
#include "dog.hpp"
#include "cat.hpp"
#include "wrongCat.hpp"

int main()
{
	std::cout << "=== TEST 1: Polymorphisme de base ===" << std::endl;
	const Animal* meta = new Animal();
	const Animal* Doggy = new Dog();
	const Animal* Garfield = new Cat();
	std::cout << "Type Dog: " << Doggy->getType() << std::endl;
	std::cout << "Type Cat: " << Garfield->getType() << std::endl;
	Garfield->makeSound();
	Doggy->makeSound();
	meta->makeSound();
	delete meta;
	delete Doggy;
	delete Garfield;


	std::cout << "\n=== TEST 2: Tableau d'animaux polymorphes ===" << std::endl;
	{
		const int size = 4;
		const Animal* animals[size] = {
			new Dog(),
			new Cat(),
			new Dog(),
			new Animal()
		};
		
		for (int i = 0; i < size; i++) {
			std::cout << "Animal " << i << " (" << animals[i]->getType() << "): ";
			animals[i]->makeSound();
		}
		
		for (int i = 0; i < size; i++) {
			delete animals[i];
		}
	}

	std::cout << "\n=== TEST 3: Objets locaux avec polymorphisme ===" << std::endl;
	{
		Animal animal;
		Dog dog;
		Cat cat;
		
		Animal& ref1 = dog;
		Animal& ref2 = cat;
		
		std::cout << "Via référence Dog: ";
		ref1.makeSound();
		std::cout << "Via référence Cat: ";
		ref2.makeSound();
	}

	std::cout << "\n=== TEST 4: wrongAnimal et wrongCat ===" << std::endl;
	{
		std::cout << "Création d'un wrongAnimal:" << std::endl;
		const wrongAnimal* wrongMeta = new wrongAnimal();
		std::cout << "Type: " << wrongMeta->getType() << std::endl;
		wrongMeta->makeSound();
		delete wrongMeta;

		std::cout << "\nCréation d'un wrongCat:" << std::endl;
		const wrongCat* wrongCatPtr = new wrongCat();
		std::cout << "Type: " << wrongCatPtr->getType() << std::endl;
		wrongCatPtr->makeSound();
		delete wrongCatPtr;

		std::cout << "\nPolymorphisme avec wrongAnimal (PAS virtuel):" << std::endl;
		const wrongAnimal* wrongPoly = new wrongCat();
		std::cout << "Type: " << wrongPoly->getType() << std::endl;
		std::cout << "Son (wrongAnimal et pas wrongCat car PAS VIRTUEL): ";
		wrongPoly->makeSound();  // Appelle wrongAnimal::makeSound() !
		delete wrongPoly;

		std::cout << "\nComparaison Animal vs wrongAnimal:" << std::endl;
		std::cout << "Animal polymorphe (virtuel):" << std::endl;
		const Animal* realCat = new Cat();
		realCat->makeSound();  // Appelle Cat::makeSound()
		delete realCat;

		std::cout << "wrongAnimal polymorphe (NON virtuel):" << std::endl;
		const wrongAnimal* fakePolyCat = new wrongCat();
		fakePolyCat->makeSound();  // Appelle wrongAnimal::makeSound()
		delete fakePolyCat;
	}

	std::cout << "\n=== FIN DES TESTS ===" << std::endl;
	return 0;
}
