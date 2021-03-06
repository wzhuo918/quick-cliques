#ifndef COMPARISON_STATIC_ORDER_MISS_H
#define COMPARISON_STATIC_ORDER_MISS_H

#include "ComparisonMISQ.h"
#include "CliqueColoringStrategy.h"

#include <vector>
#include <list>
#include "ArraySet.h"

class ComparisonStaticOrderMISS : public ComparisonMISQ
{
public:
    ComparisonStaticOrderMISS(std::vector<std::vector<char>> const &vAdjacencyMatrix, std::vector<std::vector<int>> const &vAdjacencyArray);

    virtual void InitializeOrder(std::vector<int> &P, std::vector<int> &vVertexOrder, std::vector<int> &vColors);

    virtual void GetNewOrder(std::vector<int> &vNewVertexOrder, std::vector<int> &vVertexOrder, std::vector<int> const &P, int const chosenVertex);

////    virtual void PostProcessOrder(std::vector<int> &vVertexOrder, int const chosenVertex);

protected:
    std::vector<std::vector<int>> stackOrder;
    ArraySet onlyConsider;
};

#endif //COMPARISON_STATIC_ORDER_MISS_H
