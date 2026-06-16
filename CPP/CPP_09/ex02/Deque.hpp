#pragma once

#include "PmergeMe.hpp"
#include <deque>

std::deque<size_t> getInputDeque(const char* str);
std::deque<size_t> sortWithDeque(const std::deque<size_t>& input);
std::deque<size_t> insertNbDeque(const std::deque<size_t>& sorted, size_t nb, size_t index);
std::deque<size_t> fullSortDeque(const std::deque<size_t>& lower, const std::deque<size_t>& higher, const std::vector<size_t>& sequence);
void deque_display(const std::deque<size_t>& input_deque, std::deque<size_t>& sorted_deque);
