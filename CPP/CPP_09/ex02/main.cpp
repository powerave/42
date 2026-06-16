#include "Deque.hpp"
#include "PmergeMe.hpp"

int main(int ac, char **av) {
	if (ac != 2) {
		std::cout << "wrong number of arguments" << std::endl;
		return 1;
	}

	std::vector<size_t> input;
	std::deque<size_t> input_deque;
	std::deque<size_t> sorted_deque;
	std::vector<size_t> sorted_vec;
	
	input = getInput(av[1]);
	if (input.size() < 3000) {
		std::cout << "testing needs at least 3000 numbers" << std::endl;
	}
	input_deque = getInputDeque(av[1]);
	if (input_deque.size() < 3000) {
		std::cout << "testing needs at least 3000 numbers" << std::endl;
	}
	vec_display(input, sorted_vec);
	std::cout << std::endl;
	deque_display(input_deque, sorted_deque);
	return 0;
}
