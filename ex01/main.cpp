
#include "RPN.hpp"

int main(int argc, char** argv)
{
    (void)argc; // Unused parameter
    RPN rpn;
    try
    {
        int result = rpn.Calcule(argv[1]);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}