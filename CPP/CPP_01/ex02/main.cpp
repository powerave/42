#include <string>
#include <iostream>

int	main()
{

std::string 	str = "HI THIS IS BRAIN";
std::string*	stringPTR = &str;
std::string&	stringREF = str;

std::cout << "adresse de str : " << &str << std::endl;
std::cout << "adresse de PTR : " << stringPTR << std::endl;
std::cout << "adresse de REF : " << &stringREF << std::endl;
std::cout << "valeur de str : " << str << std::endl;
std::cout << "valeur de PTR : " << *stringPTR << std::endl;
std::cout << "valeur de REF : " << stringREF << std::endl;

return 0;
}
