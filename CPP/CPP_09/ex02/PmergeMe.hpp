#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <sys/time.h>
#include <iomanip>
#include <algorithm>

struct Elements{
	size_t extra;
	std::vector<size_t> higher;
	std::vector<size_t> lower;
	bool hasExtra;
};

bool comparePairs(const std::pair<size_t, size_t>& a, const std::pair<size_t, size_t>& b);
std::vector<size_t> getInput(const char* str);
std::vector<size_t> sortJacobsthal(const std::vector<size_t>& input);
std::vector<size_t> jacobsthalSeq(size_t n);
std::vector<size_t> fullSort(const std::vector<size_t>& lower, const std::vector<size_t>& higher);
void insertNb(std::vector<size_t>& sorted, size_t nb);
void vec_display(const std::vector<size_t>& input, std::vector<size_t>& sorted_vec);
Elements build_pairs(std::vector<size_t> input);