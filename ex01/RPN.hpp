
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

class RPN
{
    public:
        RPN();
        ~RPN();
        RPN(const RPN& other);
        RPN& operator=(const RPN& other);

        int Calcule(const std::string& expression);


    private:
        std::stack<int> _stack;
        int makeOperation(long a, long b, char op) const;
};