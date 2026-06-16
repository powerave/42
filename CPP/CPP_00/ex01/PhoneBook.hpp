#pragma once

#include <iostream>
#include "Contact.hpp"

class PhoneBook 
{
private:
    static const int _maxContact = 8;
    Contact _contacts[_maxContact];
    int _currentIndex;
    int _totalContacts;

public:
    PhoneBook();
    void addContact();
    void searchContact();
    int getContactCount() const;
    bool isEmpty() const;
};
