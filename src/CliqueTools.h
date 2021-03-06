#ifndef CLIQUE_TOOLS_H
#define CLIQUE_TOOLS_H

#include <vector>
#include <list>
#include <set>

namespace CliqueTools
{
    void ComputeCliqueGraph(std::vector<std::vector<int>> &adjacencyList, std::vector<std::vector<int>> &cliqueGraphAdjacencyList, std::vector<std::vector<int>> &vertexToClique);

    void DecomposeIntoDisjointCliques(std::vector<std::vector<int>> &adjacencyList, std::vector<std::vector<int>> &cliquesToReturn);

    void FindMaximalIndependentSetInCliqueGraph(std::vector<std::vector<int>> &adjacencyList);

    bool IsMaximalClique(std::vector<std::vector<int>> &adjacencyArray, std::list<int> const&clique, bool const verbose);
    bool IsClique(std::vector<std::vector<char>> &adjacencyMatrix, std::list<int> const &clique, bool const verbose);
    bool IsIndependentSet(std::vector<std::vector<char>> &adjacencyMatrix, std::list<int> const &clique, bool const verbose);
    bool IsMaximalIndependentSet(std::vector<std::vector<int>> &adjacencyArray, std::list<int> const &vertexSet, bool const verbose);
    bool IsIndependentSet(std::vector<std::vector<int>> &adjacencyArray, std::list<int> const &vertexSet, bool const verbose);

    std::vector<int> ComputeMaximumCriticalIndependentSet(std::vector<std::vector<int>> adjacencyList);
    std::set<int> ComputeCriticalIndependentSet(std::vector<std::vector<int>> const &adjacencyList);
    std::set<int> IterativelyRemoveCriticalIndependentSets(std::vector<std::vector<int>> const &adjacencyList, std::set<int> &independentVertices);
    std::set<int> IterativelyRemoveMaximumCriticalIndependentSets(std::vector<std::vector<int>> const &adjacencyList, std::set<int> &independentVertices);
    std::set<int> ComputeMaximumCriticalIndependentSet(std::vector<std::vector<int>> const &adjacencyList, std::set<int> &independentVertices);


};

#endif //CLIQUE_TOOLS_H
