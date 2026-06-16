#pragma once
#include "animal.hpp"

class brain {
	private :
		std::string ideas[100];
	public :
		brain();
		~brain();
		brain(const brain &other);
		brain& operator=(const brain &other);
		std::string getIdea(int index) const;
		void setIdea(int index, const std::string &idea);
};
