#ifndef TESTER_STATIC_ORDER_MISS_H
#define TESTER_STATIC_ORDER_MISS_H

#include "TesterMISQ.h"
#include "CliqueColoringStrategy.h"
#include "ArraySet.h"

#include <vector>
#include <list>
#include <ctime>

class TesterStaticOrderMISS : public TesterMISQ
{
public:
    TesterStaticOrderMISS(std::vector<std::vector<char>> const &vAdjacencyMatrix, std::vector<std::vector<int>> const &vAdjacencyArray);

    virtual void InitializeOrder(std::vector<int> &P, std::vector<int> &vVertexOrder, std::vector<int> &vColors);

    virtual void GetNewOrder(std::vector<int> &vNewVertexOrder, std::vector<int> &vVertexOrder, std::vector<int> const &P, int const chosenVertex);
////    virtual void GetNewOrderNoIsolates(std::vector<int> &vNewVertexOrder, std::vector<int> &vVertexOrder, std::vector<int> const &P, int const chosenVertex);
////    virtual void ProcessOrderAfterRecursionNoIsolates(std::vector<int> &vVertexOrder, std::vector<int> &P, std::vector<int> &vColors, int const chosenVertex);

    virtual void RunRecursive(std::vector<int> &P, std::vector<int> &vVertexOrder, std::list<std::list<int>> &cliques, std::vector<int> &vColors);
////    virtual void RunRecursiveNoIsolates(std::vector<int> &P, std::vector<int> &vVertexOrder, std::list<std::list<int>> &cliques, std::vector<int> &vColors);

protected:
    ArraySet onlyConsider;
    std::vector<bool> vMarkedVertices;
};

#endif //TESTER_STATIC_ORDER_MISS_H
