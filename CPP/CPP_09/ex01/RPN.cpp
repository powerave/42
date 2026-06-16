#include "RPN.hpp"

RPN::RPN(const std::string &expr) : expression(expr) {}

RPN::~RPN() {}

RPN::RPN(const RPN& other) {
	this->expression = other.expression;
	this->stack = other.stack;
}

RPN& RPN::operator=(const RPN &other) {
	if (this != &other) {
		this->expression = other.expression;
		this->stack = other.stack;
	}
	return *this;	
}

int	RPN::process(char op, std::stack<int> &stk) {
	if (stk.size() < 2)
		throw InvalidExpressionException();
	int b = stk.top();
	stk.pop();
	int a = stk.top();
	stk.pop();
	int result;
	switch (op) {
		case '+':
			result = a + b;
			break;
		case '-':
			result = a - b;
			break;
		case '*':
			result = a * b;
			break;
		case '/':
			if (b == 0)
				throw ImpossibleCalculationException();
			result = a / b;
			break;
		default:
			throw ImpossibleCalculationException();
	}
	stk.push(result);
	return result;
}

int RPN::calculate(const std::string &expr) {
	while (!RPN::stack.empty())
		RPN::stack.pop();

	int	nb = 0;
	int sign = 0;
	int result = 0;
	for (size_t i = 0; i < expr.length(); i++) {
		if (isdigit(expr[i])) {
			int num = 0;
			size_t j = i;
			while (j < expr.length() && isdigit(expr[j])){
				num = num * 10 + (expr[j] - '0');
				j++;
			}
			stack.push(num);
			i = j - 1;
			nb++;
		}
		if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') {
			result = process(expr[i], RPN::stack);
			sign++;
		}
		if (!isdigit(expr[i]) && !(expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') && expr[i] != ' ')
			throw InvalidExpressionException();
	}
	if ((sign + 1) != nb)
		throw InvalidExpressionException();
	std::cout << "Result: " << result << std::endl;
	return result;
}
