#pragma once

#include "Bureaucrat.hpp"

class Form {
	private:
		const std::string _name;
		bool _signed;
		const int _signGrade;
		const int _execGrade;
	public:
		Form();
		Form(std::string const& name, int sign, int exec);
		~Form();
		Form(Form const& other);
		Form& operator=(Form const& other);
		void beSigned(Bureaucrat const& other);
		std::string getName() const;
};

std::ostream& operator<<(std::ostream& os, Form const& f);