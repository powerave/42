#include "NewText.hpp"

std::string	newText(std::string str1, std::string str2, std::string full) {
	std::string new_txt;
	size_t pos = 0;
	size_t next;

	while (pos != std::string::npos) {
		next = full.find(str1, pos);
		if (next != std::string::npos)
		{
			new_txt.append(full, pos, next - pos);
			new_txt += str2;
			pos = next + str1.length();
		}
		else {
			new_txt.append(full, pos, full.length() - pos);
			break ;
		}
	}
	return new_txt;
}