

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

int RPN::Calcule(const std::string& expression) {

    std::istringstream ss(expression);
    std::string value;

    while (ss >> value)
    {
       std::cout << "Processing: " << value << std::endl;

       
    }
    
    return (_stack.top());
}