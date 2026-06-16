#pragma once

#include "AForm.hpp"

class PresidentialPardonForm : public AForm {
	private:
		std::string _target;
	
	public:
		PresidentialPardonForm();
		PresidentialPardonForm(std::string const& target);
		PresidentialPardonForm(PresidentialPardonForm const & other);
		~PresidentialPardonForm();
		PresidentialPardonForm& operator=(PresidentialPardonForm const& other);
		std::string getTarget() const;
		virtual void execute(Bureaucrat const& executor) const;
};
