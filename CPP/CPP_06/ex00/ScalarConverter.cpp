#include "ScalarConverter.hpp"

ScalarConverter::ScalarConverter() {}

ScalarConverter::ScalarConverter(ScalarConverter const& other) {
	(void)other;
}

ScalarConverter::~ScalarConverter() {}

ScalarConverter& ScalarConverter::operator=(ScalarConverter const& other) {
	if (this != &other)
		return *this;
	return *this;
}

void ScalarConverter::convert(std::string const& literal) {
	if (literal == "inf" || literal == "+inf" || literal == "-inf" ||
        literal == "inff" || literal == "+inff" || literal == "-inff")
			throw InfException();
	if (literal == "nan" || literal == "nanf")
		throw NanException();
		
	char*	str;
	double	d = std::strtod(literal.c_str(), &str);

	if (*str != '\0' && !(*str == 'f' && *(str + 1) == '\0'))
	    throw ConvertionException();

	char	c = static_cast<char>(d);
	float	f =	static_cast<float>(d);
	int		i = static_cast<int>(d);

	std::cout << "char: ";
	if (d < 32 || d > 126) {
		if (d < -128 || d > 127)
			std::cout << "Impossible" << std::endl;
		else
			std::cout << "Not printable" << std::endl;
	}
	else
		std::cout << "'" << c << "'" << std::endl;
	if (d > 2147483647 || d < -2147483648)
		std::cout << "int: Impossible" << std::endl;
	else
		std::cout << "int: " << i << std::endl;
	std::cout << "float: " << std::fixed << std::setprecision(1) << f << "f" << std::endl;
	std::cout << "double: " << std::fixed << std::setprecision(1) << d << std::endl;
}
