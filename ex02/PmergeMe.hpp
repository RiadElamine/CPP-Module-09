
#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include <sstream>

class PmergeMe 
{
    private:
        std::vector<std::pair<int, int> > _vector;
        std::vector<int > _vectorMain;
        std::vector<int > _vectorPending;
        std::deque<std::pair<int, int> > _deque;
        std::deque<int > _dequeMain;
        std::deque<int > _dequePending;

        int waitnum;

        void groupElementsIntoPairs(int argc, char* argv[]);
        void printUnsortedValues() const;

        template <typename Container>
        void sortPairsInContainer(Container& container);

        template <typename Container>
        void sortContainer(Container& container);
        template <typename Container>
        void merge(const Container& left, const Container& right, Container& result);

       template <typename Container, typename SContainer>
        void splitContainer(Container& container, SContainer& mainContainer, SContainer& pendingContainer);

        void sortVector();

        void printSortedValues() const;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void sort(int argc, char **argv);
};
