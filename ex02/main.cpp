
#include "PmergeMe.hpp"

int main(int argc, char** argv)
{
    try {
        PmergeMe pmerge;
        pmerge.sort(argc, argv);

    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}