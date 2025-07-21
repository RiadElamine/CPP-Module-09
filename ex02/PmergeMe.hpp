
#pragma once
#include <vector>
#include <deque>
#include <iostream>
#include <sstream>
#include <ctime>
#include <typeinfo>

class PmergeMe 
{
    private:
        std::vector<std::pair<int, int> > _vector;
        std::vector<int > _vectorMain, _vectorPending;
        std::deque<std::pair<int, int> > _deque;
        std::deque<int > _dequeMain, _dequePending;

        std::vector<int > _unsortedValuesVector;
        std::deque<int > _unsortedValuesDeque;

        int waitnum;
        clock_t start, end;

        template <typename Container>
        void parsingError(int argc, char* argv[], Container& _C, Container& _F);

        template <typename Container>
        void printUnsortedValues(const Container& container) const;


        template <typename Container, typename SContainer>
        void groupElementsIntoPairs(Container& container, SContainer& _scontainerMain);

        template <typename Container>
        void sortPairsInContainer(Container& container);

        template <typename Container>
        void sortContainer(Container& container);
        template <typename Container>
        void merge(const Container& left, const Container& right, Container& result);

        template <typename Container, typename SContainer>
        void splitContainer(Container& container, SContainer& mainContainer, SContainer& pendingContainer);

        int generateJacobsthalIndex(int n);
    
        template <typename Container>
        void insertElement(Container& mainContainer, Container& pendingContainer);

        template <typename Container>
        void addRemainingElement(Container& mainContainer);

        template <typename Container, typename SContainer>
        void Sort(Container& container, SContainer& _containerMain, SContainer& _containerPending);

        template <typename Container>
        void printSortedValues(const Container& container) const;

        template <typename Container>
        void printtime(const Container& container) const;


    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void sort(int argc, char **argv);
};
