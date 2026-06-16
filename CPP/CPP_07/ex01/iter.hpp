#pragma once

template <typename Arr, typename F>

void iter(Arr* arr, int const length, F func) {
	for (int i = 0; i < length; i++) {
		func(arr[i]);
	}
}
