#include "Harl.hpp"

void	Harl::debug() {
	std::cout << "DEBUG level: Debug messages contain contextual information."
		"They are mostly used for problem diagnosis."
		"Example: I love having extra bacon for my 7XL-double-cheese-triple-pickle-special-ketchup burger."
		"I really do!" << std::endl;
}

void	Harl::info() {
	std::cout << "INFO level: These messages contain extensive information. They are helpful"
		"for tracing program execution in a production environment."
		"Example: I cannot believe adding extra bacon costs more money. You didn't put"
		"enough bacon in my burger! If you did, I wouldn't be asking for more!" << std::endl;
}

void	Harl::warning() {
	std::cout << "WARNING level: Warning messages indicate a potential issue in the system."
		"However, it can be handled or ignored."
		"Example: I think I deserve to have some extra bacon for free. I've been coming for"
		"years, whereas you started working here just last month" << std::endl;
}

void	Harl::error() {
	std::cerr << "ERROR level: These messages indicate that an unrecoverable error has occurred."
		"This is usually a critical issue that requires manual intervention."
		"Example: This is unacceptable! I want to speak to the manager now." << std::endl;
}

void Harl::complain(std::string level) {
	const char	*levels[] = {"DEBUG", "INFO", "WARNING", "ERROR"};
	void	(Harl::*functions[])() = {&Harl::debug, &Harl::info, &Harl::warning, &Harl::error};

	for (int i = 0; i < 4; i++) {
		if (level == levels[i]) {
			switch (i) {
			case 0:
				(this->*functions[0])();
				(this->*functions[1])();
				(this->*functions[2])();
				(this->*functions[3])();
				break;
			case 1:
				(this->*functions[1])();
				(this->*functions[2])();
				(this->*functions[3])();
				break;
			case 2:
				(this->*functions[2])();
				(this->*functions[3])();
				break;
			case 3:
				(this->*functions[3])();
				break;
			default:
				std::cout << "Nothing important" << std::endl;
			}
			return;
		}
	}
	std::cout << "[ Probably complaining about insignificant problems ]" << std::endl;
}
