#pragma once

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdexcept>

class BitcoinExchange {
	private:
		std::map<std::string, double> _data;
		void _loadData(const std::string &filename);
	public:
		BitcoinExchange();
		~BitcoinExchange();
		BitcoinExchange(const BitcoinExchange &other);
		BitcoinExchange& operator=(const BitcoinExchange &other);

		void processFile(const std::string &filename);
};
