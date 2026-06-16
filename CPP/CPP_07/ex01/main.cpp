#include "iter.hpp"
#include <iostream>

void printInt(int& i) {
    std::cout << i << " ";
}

void printString(std::string& s) {
    std::cout << s << " ";
}

void multiplyByTwo(int& i) {
    i *= 2;
}

int main() {
    int arrInt[] = {1, 2, 3, 4, 5};
    std::string arrStr[] = {"hello", "world", "!"};

    std::cout << "Original int array: ";
    iter(arrInt, 5, printInt);
    std::cout << std::endl;

    std::cout << "After multiply by 2: ";
    iter(arrInt, 5, multiplyByTwo);
    iter(arrInt, 5, printInt);
    std::cout << std::endl;

    std::cout << "String array: ";
    iter(arrStr, 3, printString);
    std::cout << std::endl;

    return 0;
}
