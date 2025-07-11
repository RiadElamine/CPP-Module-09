
#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}

// step 1: Parse the input arguments and group the elements into pairs and fill the vector and list
void PmergeMe::groupElementsIntoPairs(int argc, char* argv[])
{
    if (argc < 2) {
        throw std::invalid_argument("At least one argument is required for sorting.");
    }

    for (int i = 1; i < argc; ++i) {
        std::stringstream ss(argv[i]);
        int value;

        if (!(ss >> value) || !ss.eof() || value < 0) {
            throw std::invalid_argument("Invalid argument: " + std::string(argv[i]));
        }

        if (i % 2 != 0) {
            _vector.push_back(std::make_pair(value, -1));
            _deque.push_back(std::make_pair(value, -1));
        }
        else {
            _vector.back().second = value;
            _deque.back().second = value;
        }
    }

    waitnum = -1;
    if (_vector.back().second == -1) {
        waitnum = _vector.back().first;
        _vector.pop_back();
        _deque.pop_back();
    }

}

void PmergeMe::printUnsortedValues() const {
    std::cout << "Before:  ";
    for (std::vector<std::pair<int, int> >::const_iterator it = _vector.begin(); it != _vector.end(); ++it) {
            std::cout << it->first << " " << it->second << " ";
    }
    if (waitnum != -1) {
        std::cout << waitnum;
    }
    std::cout << std::endl;
    
}


// step 2: Sort the two elements in each pair
template <typename Container>
void PmergeMe::sortPairsInContainer(Container& container) {
    for (typename Container::iterator it = container.begin(); it != container.end(); ++it) {
        if (it->first < it->second) {
            std::swap(it->first, it->second);
        }
    }
}

// step 3: Sort larger elements in the vector and list (mregSort)
template <typename Container>
void PmergeMe::merge(const Container& left, const Container& right, Container& result) {
    
    typename Container::const_iterator itL = left.begin();
    typename Container::const_iterator itR = right.begin();
    typename Container::iterator itResult = result.begin();

    while (itL != left.end() && itR != right.end()) {
        if (itL->first <= itR->first) {
            *itResult = *itL;
            ++itL;
            ++itResult;
        } else {
            *itResult = *itR;
            ++itR;
            ++itResult;
        }
    }

    // Copy remaining elements from left
    while (itL != left.end()) {
        *itResult++ = *itL++;
    }

    // Copy remaining elements from right
    while (itR != right.end()) {
        *itResult++ = *itR++;
    }
}


template <typename Container>
void PmergeMe::sortContainer(Container& container) {

    if (container.size() < 2) {
        return;
    }

    Container right(container.begin() + container.size() / 2, container.end());
    Container left(container.begin(), container.begin() + container.size() / 2);

    sortContainer(left);
    sortContainer(right);

    merge(left, right, container);
}


// step 4: split the Container into main container and pending container
template <typename Container, typename SContainer>
void PmergeMe::splitContainer(Container& container, SContainer& mainContainer, SContainer& pendingContainer) {
    
    for (typename Container::iterator it = container.begin(); it != container.end(); ++it) {
            if (container.begin() == it && it->first >= it->second) {
                mainContainer.push_back(it->second);
                mainContainer.push_back(it->first);
                continue;
            }
            mainContainer.push_back(it->first);
            pendingContainer.push_back(it->second);
    }
}

void PmergeMe::sortVector() {

    // step 2: Sort the two elements in each pair in the vector
    sortPairsInContainer(_vector);

    // step 3: Sort larger elements in the vector
    sortContainer(_vector);

    // step 4: split the Container into main container and pending container
    splitContainer(_vector, _vectorMain, _vectorPending);

    // step 5: generate Jacobsthal Sequence for insertion order

    // step 6: insert the elements in the main container in the order of Jacobsthal Sequence

    // step 7 : add remaining element if exists

}


void PmergeMe::printSortedValues() const 
{
    std::cout << "After:   ";
    for (std::vector<int>::const_iterator it = _vectorMain.begin(); it != _vectorMain.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::sort(int argc, char **argv) {

    
    groupElementsIntoPairs(argc, argv);
    printUnsortedValues();

    sortVector();
    // sortDeque();

    printSortedValues();

    // Example: Sort the vector and deque (actual sorting logic not implemented)
    // std::sort(_list.begin(), _list.end());

}