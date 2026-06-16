#include "AMateria.hpp"
#include "Ice.hpp"
#include "Cure.hpp"
#include "ICharacter.hpp"
#include "Character.hpp"
#include "IMateriaSource.hpp"
#include "MateriaSource.hpp"

int main()
{
	IMateriaSource* src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());
	ICharacter* me = new Character("me");
	AMateria* tmp;
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("cure");
	me->equip(tmp);
	ICharacter* bob = new Character("bob");
	me->use(0, *bob);
	me->use(1, *bob);
	delete bob;
	delete me;
	delete src;
	return 0;
}

/*int main()
{
	std::cout << "=== TEST 1: MateriaSource - Learn and Create ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		
		// Apprendre des templates
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		
		// Créer des materias depuis les templates
		AMateria* ice = src->createMateria("ice");
		AMateria* cure = src->createMateria("cure");
		AMateria* unknown = src->createMateria("fire");
		
		std::cout << "Created ice: " << (ice ? ice->getType() : "NULL") << std::endl;
		std::cout << "Created cure: " << (cure ? cure->getType() : "NULL") << std::endl;
		std::cout << "Created fire: " << (unknown ? "ERROR" : "NULL (expected)") << std::endl;
		
		delete ice;
		delete cure;
		delete src;
	}

	std::cout << "\n=== TEST 2: Character - Equip and Use ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		
		ICharacter* bob = new Character("Bob");
		ICharacter* jim = new Character("Jim");
		
		AMateria* ice = src->createMateria("ice");
		AMateria* cure = src->createMateria("cure");
		
		bob->equip(ice);
		bob->equip(cure);
		
		bob->use(0, *jim);  // Ice
		bob->use(1, *jim);  // Cure
		bob->use(2, *jim);  // Vide (rien)
		
		delete bob;
		delete jim;
		delete src;
	}

	std::cout << "\n=== TEST 3: Character - Full Inventory ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		
		ICharacter* me = new Character("Me");
		
		// Remplir l'inventaire (4 slots max)
		me->equip(src->createMateria("ice"));
		me->equip(src->createMateria("ice"));
		me->equip(src->createMateria("ice"));
		me->equip(src->createMateria("ice"));
		
		// Tentative d'équiper un 5ème (devrait échouer)
		AMateria* extra = src->createMateria("ice");
		me->equip(extra);
		delete extra;  // Doit être supprimé manuellement si non équipé
		
		delete me;
		delete src;
	}

	std::cout << "\n=== TEST 4: Unequip (no delete) ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		
		ICharacter* me = new Character("Me");
		AMateria* ice = src->createMateria("ice");
		
		me->equip(ice);
		me->use(0, *me);
		
		// Unequip ne delete PAS la materia
		me->unequip(0);
		
		// On doit la delete manuellement
		delete ice;
		
		delete me;
		delete src;
	}

	std::cout << "\n=== TEST 5: Copy Constructor Deep Copy ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		
		Character* original = new Character("Original");
		original->equip(src->createMateria("ice"));
		original->equip(src->createMateria("cure"));
		
		// Copy constructor (deep copy)
		Character* copy = new Character(*original);
		
		std::cout << "Original: " << original->getName() << std::endl;
		std::cout << "Copy: " << copy->getName() << std::endl;
		
		ICharacter* target = new Character("Target");
		
		original->use(0, *target);
		copy->use(0, *target);
		
		delete original;
		delete copy;
		delete target;
		delete src;
	}

	std::cout << "\n=== TEST 6: Assignment Operator ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		
		Character bob("Bob");
		Character jim("Jim");
		
		bob.equip(src->createMateria("ice"));
		jim.equip(src->createMateria("cure"));
		
		// Assignment (deep copy)
		jim = bob;
		
		ICharacter* target = new Character("Target");
		
		bob.use(0, *target);
		jim.use(0, *target);
		
		delete target;
		delete src;
	}

	std::cout << "\n=== TEST 7: Subject Main (from PDF) ===" << std::endl;
	{
		IMateriaSource* src = new MateriaSource();
		src->learnMateria(new Ice());
		src->learnMateria(new Cure());
		
		ICharacter* me = new Character("me");
		
		AMateria* tmp;
		tmp = src->createMateria("ice");
		me->equip(tmp);
		tmp = src->createMateria("cure");
		me->equip(tmp);
		
		ICharacter* bob = new Character("bob");
		
		me->use(0, *bob);
		me->use(1, *bob);
		
		delete bob;
		delete me;
		delete src;
	}

	std::cout << "\n=== FIN DES TESTS ===" << std::endl;
	return 0;
}*/
