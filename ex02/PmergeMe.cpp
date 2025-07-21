
#include "PmergeMe.hpp"

PmergeMe::PmergeMe()
    : waitnum(-1), start(0), end(0) {}

PmergeMe::~PmergeMe() {
    _vector.clear();
    _vectorMain.clear();
    _vectorPending.clear();
    _deque.clear();
    _dequeMain.clear();
    _dequePending.clear();
}

PmergeMe::PmergeMe(const PmergeMe &other) : _vector(other._vector), 
    _vectorMain(other._vectorMain), _vectorPending(other._vectorPending),
    _deque(other._deque), _dequeMain(other._dequeMain), _dequePending(other._dequePending),
    waitnum(other.waitnum), start(other.start), end(other.end) {};

PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
    if (this != &other) {
        _vector = other._vector;
        _vectorMain = other._vectorMain;
        _vectorPending = other._vectorPending;
        _deque = other._deque;
        _dequeMain = other._dequeMain;
        _dequePending = other._dequePending;
        waitnum = other.waitnum;
        start = other.start;
        end = other.end;
    }
    return *this;
}

template <typename Container>
void PmergeMe::parsingError(int argc, char* argv[], Container& _C, Container& _F) {
    
    if (argc < 2) {
        throw std::invalid_argument("At least one argument is required for sorting.");
    }

    for (int i = 1; i < argc; ++i) {
    
        std::stringstream ss(argv[i]);
        int value;

        if (!(ss >> value) || !ss.eof() || value < 0) {
            throw std::invalid_argument("Invalid argument: " + std::string(argv[i]));
        }
        _C.push_back(value);
    }
    _F = _C;
}

// step 1: Parse the input arguments and group the elements into pairs and fill the vector and list
template <typename Container, typename SContainer>
void PmergeMe::groupElementsIntoPairs(Container& container, SContainer& _scontainerMain)
{
    int size = _scontainerMain.size();
    for (int i = 0; i < size; ++i) {
        if (i % 2 == 0) {
            container.push_back(std::make_pair(_scontainerMain[i], -1));
        } else {
            container.back().second = _scontainerMain[i];
        }
    }

    if (container.back().second == -1) {
        waitnum = container.back().first;
        container.pop_back();
    }

    _scontainerMain.clear();
}

template <typename Container>
void PmergeMe::printUnsortedValues(const Container& container) const {
    std::cout << "Before:  ";
    for (std::vector<int >::const_iterator it = container.begin(); it != container.end(); ++it) {
            std::cout << *it << " ";
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
    container.clear();
}

// step 5: generate Jacobsthal index for insertion order
int PmergeMe::generateJacobsthalIndex(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    return generateJacobsthalIndex(n - 1) + 2 * generateJacobsthalIndex(n - 2);
}

// step 6: insert the elements in the main container in the order of Jacobsthal Sequence
template <typename Container>
void PmergeMe::insertElement(Container& mainContainer, Container& pendingContainer) {
    
    int n = pendingContainer.size();
    int k = 0;
    int i = 2;
    while (1)
    {
        // step 5: Generate the Jacobsthal index for the current element
        int index = generateJacobsthalIndex(i);
        if (index >= n) {
            index = n - 1;
        }
        // step 6: Insert the element at the Jacobsthal index
        for (int j = index; j >= k; --j) {
            typename Container::iterator it = lower_bound(mainContainer.begin(), mainContainer.end(), pendingContainer[j]);
            if (it != mainContainer.end()) {
                mainContainer.insert(it, pendingContainer[j]);
            }
            else {
                mainContainer.push_back(pendingContainer[j]);
            }
        }
        k = index + 1;
        i++;
        if (index == n - 1) break;
    }

}

// step 7 : add remaining element if exists
template <typename Container>
void PmergeMe::addRemainingElement(Container& mainContainer) {
    
    if (waitnum != -1) {
        
        typename Container::iterator it = lower_bound(mainContainer.begin(), mainContainer.end(), waitnum);
        if (it != mainContainer.end()) {
            mainContainer.insert(it, waitnum);
        } else {
            mainContainer.push_back(waitnum);
        }
    }
}

//step 8: Print the time taken to sort the elements
template <typename Container>
void PmergeMe::printtime(const Container& container) const {
    double time_taken = (static_cast<double>(end - start) / static_cast<double>(CLOCKS_PER_SEC)) * 100000.0 ; 
    std::string container_type = std::string(typeid(container).name()).find("vector") != std::string::npos ? "vector" : "deque";
    std::cout << "Time to process a range of " << container.size() << " elements with std::"
                << container_type
                << " " << std::fixed << time_taken << " us" << std::endl;
}

template <typename Container, typename SContainer>
void PmergeMe::Sort(Container& container, SContainer& _containerMain, SContainer& _containerPending) {


    // step 1: Group the elements into pairs and fill the vector or deque
    groupElementsIntoPairs(container, _containerMain);

    // step 2: Sort the two elements in each pair in the vector
    sortPairsInContainer(container);

    // step 3: Sort larger elements in the vector
    sortContainer(container);

    // step 4: split the Container into main container and pending container
    splitContainer(container, _containerMain, _containerPending);

    // step 6: insert the elements in the main container in the order of Jacobsthal Sequence
    insertElement(_containerMain, _containerPending);

    // step 7 : add remaining element if exists
    addRemainingElement(_containerMain);

    //End the clock to measure time
    end = clock();

}

template <typename Container>
void PmergeMe::printSortedValues(const Container& container) const {
    std::cout << "After:   ";
    for (typename Container::const_iterator it = container.begin(); it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}


void PmergeMe::sort(int argc, char **argv) {

    // printUnsortedValues();

    //Start the clock to measure time
    start = clock();
    // Step 0: Check for parsing errors
    parsingError(argc, argv, _vectorMain, _unsortedValuesVector);
    // other steps:
    Sort(_vector, _vectorMain, _vectorPending);


    // Print the unsorted values
    printUnsortedValues(_unsortedValuesVector);

    // Print the sorted values
    printSortedValues(_vectorMain);

    //step 8: Print the time taken to sort the elements in vector
    printtime(_vectorMain);


    //Start the clock to measure time
    start = clock();
    // Step 0: Check for parsing errors
    parsingError(argc, argv, _dequeMain, _unsortedValuesDeque);
    // Print the sorted values for deque
    Sort(_deque, _dequeMain, _dequePending);
    printtime(_dequeMain);
}