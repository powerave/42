#include "PmergeMe.hpp"

std::vector<size_t> getInput(const char* str) {
	std::vector<size_t> input;
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

bool comparePairs(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b) {
    return a.second < b.second;
}

Elements build_pairs(std::vector<size_t> input) {
	Elements elements;
	elements.hasExtra = false;
	size_t i = 0;
	size_t a;
	size_t b;

	if (input.size() % 2 != 0) {
		elements.extra =  input[input.size() - 1];
		elements.hasExtra = true;
	}
	while (i + 1 < input.size()) {
		a = input[i];
		b = input[i + 1];
		if (a < b) {
			elements.higher.push_back(b);
			elements.lower.push_back(a);
		} else {
			elements.higher.push_back(a);
			elements.lower.push_back(b);
		}
		i += 2;
	}
	return elements;
}


static std::vector<size_t> generateInsertionOrder(size_t n) {
	std::vector<size_t> order;
	if (n <= 1)
	return order;

	std::vector<bool> inserted(n, false);

	std::vector<size_t> jSeq;
	jSeq.push_back(1); // commence q 1 car 1 + 2*0 = 1
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

std::vector<size_t> fullSort(const std::vector<size_t>& lower, const std::vector<size_t>& higher) {
	std::vector<size_t> sorted = higher;
	if (lower.empty()) return sorted;

	sorted.insert(sorted.begin(), lower[0]);

	std::vector<size_t> order = generateInsertionOrder(lower.size());
	for (size_t i = 0; i < order.size(); i++)
		insertNb(sorted, lower[order[i]]);
	return sorted;
}

void insertNb(std::vector<size_t>& sorted, size_t nb) {
	std::vector<size_t>::iterator it = std::lower_bound(sorted.begin(), sorted.end(), nb);
	sorted.insert(it, nb);
}

std::vector<size_t> jacobsthalSeq(size_t n) {
	std::vector<size_t> seq;
	seq.push_back(0);
	seq.push_back(1);

	while (seq.back() < n)
    {
        size_t next = seq[seq.size() - 1] + 2 * seq[seq.size() - 2];
        seq.push_back(next);
    }
	return seq;
}

std::vector<size_t> sortJacobsthal(const std::vector<size_t>& input){
	if (input.size() <= 1)
		return input;

	Elements elems = build_pairs(input);

	std::vector<size_t> originalHigher = elems.higher;
	std::vector<size_t> originalLower  = elems.lower;

	elems.higher = sortJacobsthal(elems.higher);

	std::vector<size_t> reorderedLower;
	for (size_t i = 0; i < elems.higher.size(); i++) {
		for (size_t j = 0; j < originalHigher.size(); j++) {
			if (originalHigher[j] == elems.higher[i]) {
				reorderedLower.push_back(originalLower[j]);
				break;
			}
		}
	}

	std::vector<size_t> seq = jacobsthalSeq(reorderedLower.size());
	std::vector<size_t> sorted = fullSort(reorderedLower, elems.higher);
	if (elems.hasExtra)
		insertNb(sorted, elems.extra);
	return sorted;
}


void vec_display(const std::vector<size_t>& input, std::vector<size_t>& sorted_pair) {
	std::cout << "Before: ";
	for (size_t i = 0; i < input.size(); i++) {
		std::cout << input[i] << " ";
	}
	std::cout << std::endl;

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	sorted_pair = sortJacobsthal(input);
	clock_gettime(CLOCK_MONOTONIC, &end);
	std::cout << "After: ";
	for (size_t i = 0; i < sorted_pair.size(); i++)
		std::cout << sorted_pair[i] << " ";
	std::cout << std::endl;
	double time_taken = (end.tv_sec - start.tv_sec) * 1e6 + (end.tv_nsec - start.tv_nsec) / 1000.0;
	std::cout << "Time to process a range of " << input.size() << " elements with std::vector : " << std::fixed << std::setprecision(5) << time_taken << " us" << std::endl;
}
