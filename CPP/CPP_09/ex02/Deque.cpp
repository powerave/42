#include "Deque.hpp"

std::deque<size_t> getInputDeque(const char* str) {
	std::deque<size_t> input;
	for (size_t i = 0; i < strlen(str); i++) {
		size_t num = 0;
		if (!isdigit(str[i]) && str[i] != ' ')
		{
			std::cout << "Error: Invalid character in input." << std::endl;
			exit(1);
		}
		while(str[i] == ' ' && i < strlen(str))
			i++;
		size_t j = i;
		while(isdigit(str[j]) && i < strlen(str)) {
			num = num * 10 + (str[j] - '0');
			j++;
		}
		i = j - 1;
		input.push_back(num);	
	}
	return input;
}

std::deque<size_t> sortWithDeque(const std::deque<size_t>& input) {
	if (input.size() <= 1)
		return input;

	std::deque<size_t> lower;
	std::deque<size_t> higher;
	bool hasExtra = false;
	size_t lastNb = 0;

	if (input.size() % 2 != 0) {
		lastNb = input[input.size() - 1];
		hasExtra = true;
	}

	for (size_t i = 0; i < input.size(); i += 2) {
		if (i + 1 < input.size()) {
			size_t a = input[i];
			size_t b = input[i + 1];
			if (a < b) {
				lower.push_back(a);
				higher.push_back(b);
			}
			else {
				lower.push_back(b);
				higher.push_back(a);
			}
		}
}

higher = sortWithDeque(higher);

std::vector<size_t> sequence = jacobsthalSeq(lower.size());
std::deque<size_t> sorted = fullSortDeque(lower, higher, sequence);
if (hasExtra)
	sorted = insertNbDeque(sorted, lastNb, 0);
return sorted;
}

std::deque<size_t> insertNbDeque(const std::deque<size_t>& sorted, size_t nb, size_t index) {
	std::deque<size_t> result = sorted;
	if (sorted.empty()) {
		result.push_back(nb);
		return result;
	}
	
	size_t left = index;
	size_t right = sorted.size();
	
	while (left < right) {
		size_t mid = (left + right) / 2;
		if (sorted[mid] < nb)
			left = mid + 1;
		else
			right = mid;
	}
	
	result.insert(result.begin() + left, nb);
	return result;
}

static std::vector<size_t> generateInsertionOrderDeque(size_t n) {
	std::vector<size_t> order;
	if (n <= 1) return order;

	std::vector<bool> inserted(n, false);
	std::vector<size_t> jSeq;
	jSeq.push_back(1);
	jSeq.push_back(3);
	while (jSeq.back() < n) {
		size_t next = jSeq[jSeq.size()-1] + 2 * jSeq[jSeq.size()-2];
		jSeq.push_back(next);
	}

	size_t prev = 0;
	for (size_t k = 0; k < jSeq.size(); k++) {
		size_t curr = (jSeq[k] < n) ? jSeq[k] : n - 1;
		for (size_t i = curr; i > prev; i--) {
			if (!inserted[i]) {
				order.push_back(i);
				inserted[i] = true;
			}
		}
		prev = jSeq[k];
		if (prev >= n - 1) break;
	}
	return order;
}

std::deque<size_t> fullSortDeque(const std::deque<size_t>& lower, const std::deque<size_t>& higher, const std::vector<size_t>& sequence) {
	(void)sequence;
	std::deque<size_t> sorted = higher;
	if (lower.empty()) return sorted;

	sorted = insertNbDeque(sorted, lower[0], 0);

	std::vector<size_t> order = generateInsertionOrderDeque(lower.size());
	for (size_t i = 0; i < order.size(); i++)
		sorted = insertNbDeque(sorted, lower[order[i]], 0);
	return sorted;
}

void deque_display(const std::deque<size_t>& input_deque, std::deque<size_t>& sorted_deque) {
	std::cout << "Before: ";
	for (size_t i = 0; i < input_deque.size(); i++) {
		std::cout << input_deque[i] << " ";
	}
	std::cout << std::endl;

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	sorted_deque = sortWithDeque(input_deque);
	clock_gettime(CLOCK_MONOTONIC, &end);
	std::cout << "After: ";
	for (size_t i = 0; i < sorted_deque.size(); i++) {
		std::cout << sorted_deque[i] << " ";
	}
	std::cout << std::endl;
	double time_taken = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1000.0;
	std::cout << "Time to process a range of " << input_deque.size() << " elements with std::deque : " << std::fixed << std::setprecision(5) << time_taken << " us" << std::endl;
}
