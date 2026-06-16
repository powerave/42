#include "span.hpp"

Span::Span(unsigned int N) : _N(N) {}
Span::~Span() {}
Span::Span(const Span& other) : _N(other._N), _numbers(other._numbers) {}
Span& Span::operator=(const Span& other) {
	if (this != &other) {
		_N = other._N;
		_numbers = other._numbers;
	}
	return *this;
}

void Span::addNumber(int number) {
	if (_numbers.size() >= _N)
		throw std::runtime_error("Span is already full");
	_numbers.push_back(number);
}

int Span::shortestSpan() {
	if (_numbers.size() < 2)
		throw std::runtime_error("Not enough numbers to find a Span");
	std::vector<int> sortedNumbers = _numbers;
	std::sort(sortedNumbers.begin(), sortedNumbers.end());
	int minSpan = INT_MAX;
	for (size_t i = 1; i < sortedNumbers.size(); ++i) {
		int span = sortedNumbers[i] - sortedNumbers[i - 1];
		if (span < minSpan)
			minSpan = span;
	}
	return minSpan;
}

int Span::longestSpan() {
	if (_numbers.size() < 2)
		throw std::runtime_error("Not enough numbers to find a Span");
	int minNumber = *std::min_element(_numbers.begin(), _numbers.end());
	int maxNumber = *std::max_element(_numbers.begin(), _numbers.end());
	return maxNumber - minNumber;
}
