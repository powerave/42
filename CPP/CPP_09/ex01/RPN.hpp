#pragma once

#include <string>
#include <stack>
#include <exception>
#include <iostream>

class InvalidExpressionException : public std::exception {
	const char* what() const throw() {
		return "Invalid Expression";
	}
};

class ImpossibleCalculationException : public std::exception {
	const char* what() const throw() {
		return "Impossible Calculation";
	}
};

class RPN {
	private:
		std::string expression;
		std::stack<int> stack;
	public:
		RPN(const std::string &expr);
		~RPN();
		RPN(const RPN& other);
		RPN& operator=(const RPN &other);

		int calculate(const std::string &expr);
		int process(char op, std::stack<int> &stk);
};
