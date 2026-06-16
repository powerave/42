#include "Contact.hpp"
#include "PhoneBook.hpp"

int main() {
    PhoneBook phoneBook;
	std::string command;
	
	while (true)
	{
		std::cout << "type: ADD | SEARCH | EXIT" << std::endl;
		std::cout << "_________________________" << std::endl;
		if (!std::getline(std::cin, command))
			return 1;
		if (command == "ADD") 
			phoneBook.addContact();
		else if (command == "SEARCH")
			phoneBook.searchContact();
		else if (command == "EXIT")
			break;
		else
			continue;
	}
	return 0;
}
