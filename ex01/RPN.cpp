

#include "RPN.hpp"

RPN::RPN() {}

RPN::~RPN() {}

RPN::RPN(const RPN& other) : _stack(other._stack) {}

RPN& RPN::operator=(const RPN& other) {
    if (this != &other) {
        _stack = other._stack;
    }
    return *this;
}

void addSpacesWithStream(const std::string& expression, std::stringstream &oss) {

    for (std::string::const_iterator it = expression.begin(); it != expression.end(); ++it) {
        if (*it == '+' || *it == '-' || *it == '*' || *it == '/') {
            oss << ' ' << *it << ' ';
        } else {
            oss << *it;
        }
    }
}


int RPN::Calcule(const std::string& expression) {

    std::stringstream ss;
    std::string expr;

    if (expression.empty()) {
        throw std::invalid_argument("Expression is empty");
    }
    addSpacesWithStream(expression, ss);
    while (ss >> expr)
    {
        if (isdigit(expr[0]))
        {
            std::stringstream ss_num(expr);
            int num;
            if (!(ss_num >> num)) {
                throw std::invalid_argument("Overflow or Undeflow error");
            }
            if (!ss_num.eof()) {
                throw std::invalid_argument("Invalid number: " + expr);
            }
            _stack.push(num);
            continue;
        }
        else if ((expr.find_first_of("+-*/") == 0))
        {
            for (size_t i = 0; i < expr.size(); ++i) {
                if (_stack.size() < 2) {
                    throw std::invalid_argument("Not enough operands for operation: " + expr);
                }
                int b = _stack.top();
                _stack.pop();
                int a = _stack.top();
                _stack.pop();
                switch (expr[i]) {
                    case '+': _stack.push(a + b); break;
                    case '-': _stack.push(a - b); break;
                    case '*': _stack.push(a * b); break;
                    case '/':
                    {
                        if (b == 0) {
                            throw std::invalid_argument("Division by zero error");
                        }
                        _stack.push((a / b));
                        break;
                    };
                    default:
                        throw std::invalid_argument("Invalid operator: " + expr);
                }
            }
        }
        else
        {
            throw std::invalid_argument("Invalid token: " + expr);
        }
        if (_stack.size() > 1) {
            throw std::invalid_argument("Invalid :( Too many operands left in stack after evaluation");
        }
    }
    return (_stack.top());
}