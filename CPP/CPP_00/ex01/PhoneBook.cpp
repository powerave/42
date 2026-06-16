#include "PhoneBook.hpp"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>

PhoneBook::PhoneBook() : _currentIndex(0), _totalContacts(0) {
}

void PhoneBook::addContact()
{
	std::string lastName;
	std::string firstName;
	std::string nickName;
	std::string phoneNumber;
	std::string darkestSecret;

	std::cout << "Enter contact last name: " << std::endl;
	std::getline(std::cin, lastName);
	std::cout << "Enter contact first name: " << std::endl;
	std::getline(std::cin, firstName);
	std::cout << "Enter contact nickname: " << std::endl;
	std::getline(std::cin, nickName);
	std::cout << "Enter contact phone number: " << std::endl;
	std::getline(std::cin, phoneNumber);
	std::cout << "Enter contact darkest secret <(;..;)> : " << std::endl;
	std::getline(std::cin, darkestSecret);

	if (lastName.empty() || firstName.empty() || nickName.empty()
		|| phoneNumber.empty() || darkestSecret.empty())
	{
		std::cout << "Error: All fields must be filled." << std::endl;
		return;
	}

	Contact newContact(firstName, lastName, nickName, phoneNumber, darkestSecret);
	_contacts[_currentIndex] = newContact;
	
	_currentIndex = (_currentIndex + 1) % _maxContact;
	if (_totalContacts < _maxContact) {
		_totalContacts++;
	}
	
	std::cout << "Contact added successfully!" << std::endl;
}

void PhoneBook::searchContact()
{

	int index;

	if (_totalContacts == 0) {
		std::cout << "No contacts in phonebook." << std::endl;
		return;
	}
	std::cout << std::setw(10) << "Index" << "|";
	std::cout << std::setw(10) << "First Name" << "|";
	std::cout << std::setw(10) << "Last Name" << "|";
	std::cout << std::setw(10) << "Nickname" << std::endl;
	
	for (int i = 0; i < _totalContacts; i++) {
		std::cout << std::setw(10) << i << "|";
	
		std::string firstName = _contacts[i].getFirstName();
		if (firstName.length() > 10) {
			firstName = firstName.substr(0, 9) + ".";
		}
		std::cout << std::setw(10) << firstName << "|";
		
		std::string lastName = _contacts[i].getLastName();
		if (lastName.length() > 10) {
			lastName = lastName.substr(0, 9) + ".";
		}
		std::cout << std::setw(10) << lastName << "|";
		
		std::string nickname = _contacts[i].getNickname();
		if (nickname.length() > 10) {
			nickname = nickname.substr(0, 9) + ".";
		}
		std::cout << std::setw(10) << nickname << std::endl;
	}
	
	std::cout << "Enter the index of the contact to display: " << std::endl;
	std::string input;
	if (!std::getline(std::cin, input))
		return;
	std::istringstream iss(input);
	if (!(iss >> index) || index < 0 || index >= _totalContacts)
	{
        std::cout << "Invalid index." << std::endl;
        return; 
	}
		std::cout << "First Name: " << _contacts[index].getFirstName() << std::endl;
		std::cout << "Last Name: " << _contacts[index].getLastName() << std::endl;
		std::cout << "Nickname: " << _contacts[index].getNickname() << std::endl;
		std::cout << "Phone Number: " << _contacts[index].getPhoneNumber() << std::endl;
		std::cout << "Darkest Secret: " << _contacts[index].getDarkestSecret() << std::endl;
}

int PhoneBook::getContactCount() const {
	return _totalContacts;
}

bool PhoneBook::isEmpty() const {
	return _totalContacts == 0;
}
