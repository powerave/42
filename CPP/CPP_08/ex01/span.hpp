#pragma once

#include <vector>
#include <exception>
#include <stdexcept>
#include <algorithm>
#include <climits>
#include <iostream>

class Span {
	private:
		unsigned int _N;
		std::vector<int> _numbers;
	public:
		Span(unsigned int N);
		~Span();
		Span(const Span& other);
		Span& operator=(const Span& other);
		void addNumber(int number);
		template <typename Iterator>
		void addNumbers(Iterator begin, Iterator end) {
			for (Iterator it = begin; it != end; ++it) {
				if (_numbers.size() >= _N)
					throw std::runtime_error("Span is already full");
				_numbers.push_back(*it);
			}
		}
		int shortestSpan();
		int longestSpan();
};
