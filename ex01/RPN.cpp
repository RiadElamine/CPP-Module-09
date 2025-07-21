

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

int RPN::makeOperation(long a, long b, char op) const {
    
    long result = 0;
    switch (op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        default:
        {
            if (b == 0) {
                throw std::invalid_argument("Division by zero error");
            }
            if (a == std::numeric_limits<int>::min() && b == -1) {
                throw std::overflow_error("Overflow on division (INT_MIN / -1)");
            }
            return (static_cast<int>((a / b)));
        };
    }
    if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
    {
        throw std::overflow_error("Overflow or Underflow error");
    }
    return (static_cast<int>(result));
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
            if (num < 0 || num > 9) {
                throw std::invalid_argument("Number out of range: " + expr);
            }
            _stack.push(num);
            continue;
        }
        else if ((expr.find_first_of("+-*/") == 0) && _stack.size() >= 2)
        {
            int b = _stack.top();
            _stack.pop();
            int a = _stack.top();
            _stack.pop();

            _stack.push(makeOperation(a, b, expr[0]));
        }
        else
        {
            throw std::invalid_argument("Invalid token: " + expr);
        }
    }

    if (_stack.size() > 1) {
        throw std::invalid_argument("Invalid :( Too many numbers left in stack");
    }

    return (_stack.top());
}