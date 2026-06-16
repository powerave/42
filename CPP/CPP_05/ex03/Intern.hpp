#pragma once

#include "AForm.hpp"



class Intern {
	public:
		Intern();
		Intern(Intern const& other);
		~Intern();
		Intern& operator=(Intern const& other);
		AForm* makeForm(std::string const& name, std::string const& target);
};
