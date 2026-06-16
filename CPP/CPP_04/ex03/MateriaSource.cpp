#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
	for(int i = 0; i < 4; i++)
		_Templates[i] = NULL;
	std::cout << "MateriaSource default constructor called" << std::endl;
}

MateriaSource::~MateriaSource() {
	for(int i = 0; i < 4; i++)
		if (_Templates[i])
			delete _Templates[i];
	std::cout << "MateriaSource destructor called" << std::endl;
}

void MateriaSource::learnMateria(AMateria* m) {
	if (!m)
		return;
	for (int i = 0; i < 4; i++) {
		if (!_Templates[i]) {
			_Templates[i] = m->clone();
			std::cout << "Learned materia: " << m->getType() << std::endl;
			delete m;
			return;
		}
	}
	std::cout << "Cannot learn more Materia" << std::endl;
	delete m;
}

AMateria* MateriaSource::createMateria(std::string const &type) {
	for(int i = 0; i < 4; i++) {
		if (_Templates[i] && _Templates[i]->getType() == type)
			return _Templates[i]->clone();
	}
	return NULL;
}