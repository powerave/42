#pragma once

#include <string>
#include <iostream>

template <typename T>
void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

template <typename M>
M min(M& a, M& b) {
	if (a == b) {
		return b;
	}
	return (a < b) ? a : b;
}

template <typename X>
X max(X& a, X& b) {
	if (a == b) {
		return b;
	}
	return (a > b) ? a : b;
}
