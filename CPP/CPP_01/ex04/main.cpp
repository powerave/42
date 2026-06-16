#include "NewText.hpp"

int	main(int ac, char **av) {	
	if (ac != 4)
	{
		std::cout << "Not enough or too many arguments" << std::endl;
		return 1;
	}
	std::string	filename = av[1];
	std::string str1 = av[2];
	std::string str2 = av[3];
	
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		std::cout << "Couldn't open file" << std::endl;
		return 1;
	}

	std::string FullText;
	std::string line;
	while (std::getline(file, line)) {
		FullText += line;
		if (!file.eof())
			FullText += "\n";
	}
	file.close();
	std::string NewTxt = newText(str1, str2, FullText);
	std::string outfilename = filename + ".replace";
	std::ofstream outfile(outfilename.c_str());
	if (!outfile.is_open()){
		std::cout << "Couldn't create file" << std::endl;
		return 1;
	}
	outfile << NewTxt;
	outfile.close();
	return 0;
}