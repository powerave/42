#include "ICharacter.hpp"

std::ostream& operator<<(std::ostream &os, ICharacter const &m) {
	return os << m.getName();
}
