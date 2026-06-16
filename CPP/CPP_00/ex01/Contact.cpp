#include "Contact.hpp"

Contact::Contact() : _firstName(""), _lastName(""), _nickname(""), _phoneNumber(""), _darkestSecret("") {
}

Contact::Contact(const std::string& firstName, const std::string& lastName, 
                const std::string& nickname, const std::string& phoneNumber, 
                const std::string& darkestSecret) 
    : _firstName(firstName), _lastName(lastName), _nickname(nickname), 
      _phoneNumber(phoneNumber), _darkestSecret(darkestSecret) {
}

std::string Contact::getFirstName() const {
    return _firstName;
}

std::string Contact::getLastName() const {
    return _lastName;
}

std::string Contact::getNickname() const {
    return _nickname;
}

std::string Contact::getPhoneNumber() const {
    return _phoneNumber;
}

std::string Contact::getDarkestSecret() const {
    return _darkestSecret;
}

void Contact::setFirstName(const std::string& firstName) {
    this->_firstName = firstName;
}

void Contact::setLastName(const std::string& lastName) {
    this->_lastName = lastName;
}

void Contact::setNickname(const std::string& nickname) {
    this->_nickname = nickname;
}

void Contact::setPhoneNumber(const std::string& phoneNumber) {
    this->_phoneNumber = phoneNumber;
}

void Contact::setDarkestSecret(const std::string& darkestSecret) {
    this->_darkestSecret = darkestSecret;
}

bool Contact::isEmpty() const {
    return _firstName.empty() && _lastName.empty() && _nickname.empty() && 
           _phoneNumber.empty() && _darkestSecret.empty();
}
