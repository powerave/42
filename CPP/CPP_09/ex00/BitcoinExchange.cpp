#include "BitcoinExchange.hpp"
#include <cstdlib>

BitcoinExchange::BitcoinExchange() {
	_loadData("data.csv");
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other) {
	_data = other._data;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange &other) {
	if (this != &other) {
		_data = other._data;
	}
	return *this;
}

void BitcoinExchange::_loadData(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file.");
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) {
		size_t pos = line.find(',');
		if (pos != std::string::npos) {
			std::string date = line.substr(0, pos);
			double value = std::atof(line.substr(pos + 1).c_str());
			_data[date] = value;
		}
	}
	file.close();
}

void BitcoinExchange::processFile(const std::string &filename) {
	std::ifstream file(filename.c_str());
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file.");
	}
	std::string line;
	std::getline(file, line);
	while (std::getline(file, line)) {
		size_t pos = line.find('|');
		if (pos == std::string::npos) {
			std::cout << "Error: Bad input => " << line << std::endl;
			continue;
		}
		std::string date = line.substr(0, pos);
		std::string BTCcount = line.substr(pos + 1);

		date.erase(0, date.find_first_not_of(" \t"));
		BTCcount.erase(0, BTCcount.find_first_not_of(" \t"));
		date.erase(date.find_last_not_of(" \t") + 1);
		BTCcount.erase(BTCcount.find_last_not_of(" \t") + 1);
		double amount = std::atof(BTCcount.c_str());
		if (amount < 0) {
			std::cout << "Error: Not a positive number." << std::endl;
			continue;
		}
		if (amount > 1000) {
			std::cout << "Error: Too large a number." << std::endl;
			continue;
		}
		std::map<std::string, double>::iterator it = _data.upper_bound(date);
		if (it == _data.begin()) {
			std::cout << "Error: Too early for date " << date << std::endl;
			continue;
		}
		--it;
		double result = amount * it->second;
		std::cout << date << " => " << amount << " = " << result << std::endl;
	}
	file.close();
}
