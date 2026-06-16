#include "ConvertionException.hpp"

const char* ConvertionException::what() const throw() {
	return "Conversion impossible";
}
