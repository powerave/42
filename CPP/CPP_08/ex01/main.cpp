#include "span.hpp"

int main () {
	Span sp = Span(5);
	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);
	std::cout << sp.shortestSpan() << std::endl;
	std::cout << sp.longestSpan() << std::endl;
	try {
		std::vector<int> largeVec;
		for (int i = 0; i < 42; i++)
			largeVec.push_back(i);
		Span largeSpan(42);
		largeSpan.addNumbers(largeVec.begin(), largeVec.end());
		std::cout << "Shortest in largeSpan: " << largeSpan.shortestSpan() << std::endl;
		std::cout << "Longest in largeSpan: " << largeSpan.longestSpan() << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return 0;
}
