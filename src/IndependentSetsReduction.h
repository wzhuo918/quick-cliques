#ifndef INDEPENDENT_SETS_REDUCTION_H
#define INDEPENDENT_SETS_REDUCTION_H

// local includes
#include "VertexSets.h"
#include "Isolates.h"

// system includes
#include <vector>
#include <cstring> // memcopy
#include <iostream>
#include <utility>

#define REMOVE_ISOLATES

class IndependentSetsReduction : public VertexSets
{
public:
    IndependentSetsReduction(std::vector<std::vector<int>> &adjacencyList);
    virtual ~IndependentSetsReduction();

    IndependentSetsReduction           (IndependentSetsReduction const &sets) = delete;
    IndependentSetsReduction& operator=(IndependentSetsReduction const &sets) = delete;

    void AddToAdjacencyList     (std::vector<std::pair<int,int>> const &vEdges);
    void RemoveFromAdjacencyList(std::vector<std::pair<int,int>> const &vEdges);

    virtual void MoveFromPToR(int const vertexInP) __attribute__((always_inline));
    virtual void MoveFromPToR(std::list<int> &partialClique, int const vertexInP);
    virtual void MoveFromRToX(int const vertexInP) __attribute__((always_inline));
    virtual void MoveFromRToX(std::list<int> &partialClique, int const vertexInP);

    virtual void ReturnVerticesToP(std::vector<int> const &vVertices) __attribute__((always_inline));

    std::vector<int> ChoosePivot() const __attribute__((always_inline));
    bool InP(int const vertex) const __attribute__((always_inline));

    bool PIsEmpty() const __attribute__((always_inline));
    bool XAndPAreEmpty() const __attribute__((always_inline));

    virtual size_t SizeOfX() const { return beginP - beginX; }
    virtual size_t SizeOfP() const { return beginR - beginP; }

    virtual size_t GetGraphSize() const { return m_AdjacencyList.size(); }

    void Initialize();

    virtual void PrintSummary(int const line) const;

    virtual bool GetNextTopLevelPartition();

    virtual void GetTopLevelPartialClique(std::list<int> &partialClique) const
    {
        for (int const cliqueVertex : isolates.GetIsolates()) {
            partialClique.push_back(cliqueVertex);
        }
    }

    virtual int GetNextVertexToEvaluate(std::vector<int> &vVertices)
    {
#ifdef REMOVE_ISOLATES
        int const vertexToRemove(isolates.NextVertexToRemove(vVertices));
        for (int &vertex : vVertices) {
            if (vertex == vertexToRemove) {
                vertex = vVertices.back();
                vVertices.pop_back();
                return vertexToRemove;
            }
        }
#endif
        return VertexSets::GetNextVertexToEvaluate(vVertices);
    }

    std::string CheckP()
    {
        std::string errorString;
        ArraySet const &inGraph(isolates.GetInGraph());
        for (int i = beginP; i < beginR; i++) {
            int const vertex(vertexSets[i]);
            if (!inGraph.Contains(vertex)) {
                errorString += "Mismatch: " + std::to_string(vertex) + " in P, but not graph. ";
            }
        }

        if (SizeOfP() != inGraph.Size()) {
            errorString += "Mismatch: Size of P=" + std::to_string(SizeOfP()) + ", Graph Size=" + std::to_string(inGraph.Size());
            for (int const vertex : inGraph) {
                if (!InP(vertex)) {
                    errorString += "Mismatch: " + std::to_string(vertex) + " in graph, but not P. ";
                }
            }
        }

        if (!errorString.empty()) return errorString;
        return "Ok";
    }

protected: // methods

    void StoreGraphChanges(std::vector<int> &vCliqueVertices, std::vector<int> &vOtherRemoved, std::vector<std::pair<int,int>> &vAddedEdges);
    void RetrieveGraphChanges(std::vector<int> &vCliqueVertices, std::vector<int> &vOtherRemoved, std::vector<std::pair<int,int>> &vAddedEdges);

protected: // members
    int beginX;
    int beginP;
    int beginR;
    std::vector<std::vector<int>> &m_AdjacencyList;
    std::vector<int> vertexSets;
    std::vector<int> vertexLookup;
    std::vector<int> degree;
    std::vector<SetDelineator> m_lDelineators;
    Isolates isolates;
    std::vector<std::vector<int>> vvCliqueVertices;
    std::vector<std::vector<int>> vvOtherRemovedVertices;
    std::vector<std::vector<std::pair<int,int>>> vvAddedEdges;
};

inline void IndependentSetsReduction::ReturnVerticesToP(std::vector<int> const &vVertices)
{
#if 1 // TODO/DS: PUT BACK!
    for (int const vertex : vVertices) {
        int const vertexLocation = vertexLookup[vertex];
        beginP--;
        vertexSets[vertexLocation] = vertexSets[beginP];
        vertexSets[beginP] = vertex;
        vertexLookup[vertex] = beginP;
        vertexLookup[vertexSets[vertexLocation]] = vertexLocation;
    }

#ifdef REMOVE_ISOLATES
    isolates.ReplaceAllRemoved(vVertices);
#endif
#endif
}

inline void IndependentSetsReduction::MoveFromPToR(int const vertex)
{
    std::cout << "FATAL ERROR: This function should not be called" << std::endl;
    exit(1);
}

// DONE, need to verify
inline void IndependentSetsReduction::MoveFromPToR(std::list<int> &partialClique, int const vertex)
{
////    PrintSummary(__LINE__);
////    std::cout << "Moving " << vertex << " to R" << std::endl << std::flush;
////    std::cout << "Location of " << vertex << " is " << vertexLocation << std::endl << std::flush;

    //swap vertex into R and update beginR
////    std::cout << "Moving swapping with location: " << beginR << std::endl << std::flush;

    //// added code for reduction

    std::vector<int> vCliqueVertices;
    std::vector<int> vOtherRemoved;
    std::vector<std::pair<int,int>> vAddedEdges;

////    std::cout << __LINE__ << ": " << CheckP() << std::endl;

////    std::cout << "Removing vertex " << vertex << " and neighbors: ";
////    for (int const removedVertex : vOtherRemoved) {
////        std::cout << removedVertex << " ";
////    }
////    std::cout << std::endl;

    isolates.RemoveVertexAndNeighbors(vertex, vOtherRemoved);
    vCliqueVertices.push_back(vertex);

#ifdef REMOVE_ISOLATES
    isolates.RemoveAllIsolates(0, vCliqueVertices, vOtherRemoved, vAddedEdges);
////    std::cout << "Removed " << vCliqueVertices.size() + vOtherRemoved.size() << " vertices in reduction" << std::endl;

    partialClique.insert(partialClique.end(), vCliqueVertices.begin(), vCliqueVertices.end());
    AddToAdjacencyList(vAddedEdges);
#else
    partialClique.push_back(vertex);
#endif
////    std::cout << "Moving vertices ";
////    for (int const cliqueVertex : vCliqueVertices) {
////        std::cout << cliqueVertex << " ";
////    }
////    std::cout << " to R." << std::endl;

    // save state
    m_lDelineators.emplace_back(VertexSets::SetDelineator(beginX, beginP, beginR));

    // Make new indices into vertexSets for recursive call
    // initially the new sets X, P, and R are empty.
    int newBeginX = beginX;
    int newBeginP = beginP;
    int newBeginR = beginR;

    // for each neighbor of vertex, ask if it is in X or P,
    // if it is, swap it out. Otherwise, leave it in.
    for (int const cliqueVertex : vCliqueVertices) {
        // first move the vertex to R.
////        std::cout << "Moving clique vertex " << cliqueVertex << " to R" << std::endl;
        int const vertexLocation = vertexLookup[cliqueVertex];
        newBeginR--;
        vertexSets[vertexLocation] = vertexSets[newBeginR];
        vertexLookup[vertexSets[newBeginR]] = vertexLocation;
        vertexSets[newBeginR] = cliqueVertex;
        vertexLookup[cliqueVertex] = newBeginR;
        for (int const neighbor : m_AdjacencyList[cliqueVertex]) {
            int const neighborLocation = vertexLookup[neighbor];
////            std::cout << "Evaluating neighbor " << neighbor << std::endl << std::flush;

            // if in X
            if (neighborLocation >= beginX && neighborLocation < newBeginX) {
////                std::cout << "    Moving neighbor " << neighbor << " out of X" << std::endl;
                // swap out of new X territory
                vertexSets[neighborLocation] = vertexSets[newBeginX];
                vertexLookup[vertexSets[newBeginX]] = neighborLocation;
                vertexSets[newBeginX] = neighbor;
                vertexLookup[neighbor] = newBeginX;
                newBeginX++;
            }

            //if in P
            else if (neighborLocation >= beginP && neighborLocation < newBeginR) {
////                std::cout << "    Moving neighbor " << neighbor << " out of P" << std::endl;
                // swap out of new P territory
                newBeginR--;
                vertexSets[neighborLocation] = vertexSets[newBeginR];
                vertexLookup[vertexSets[newBeginR]] = neighborLocation;
                vertexSets[newBeginR] = neighbor;
                vertexLookup[neighbor] = newBeginR;
            }
        }

        // iterate until X and P are the common non-neighborhood
        beginX = newBeginX;
        beginP = newBeginP;
        beginR = newBeginR;
    }

    // save state
    StoreGraphChanges(vCliqueVertices, vOtherRemoved, vAddedEdges);

////    std::cout << __LINE__ << ": " << CheckP() << std::endl;

////    PrintSummary(__LINE__);
}

inline void IndependentSetsReduction::MoveFromRToX(int const vertex)
{
    std::cout << "FATAL ERROR: This function should not be called" << std::endl;
    exit(1);
}

// DONE: need to verify
inline void IndependentSetsReduction::MoveFromRToX(std::list<int> &partialClique, int const vertex)
{
    SetDelineator const &delineator = m_lDelineators.back();

////    std::cout << __LINE__ << ": " << CheckP() << std::endl;

    beginX = delineator.m_BeginX;
    beginP = delineator.m_BeginP;
    beginR = delineator.m_BeginR;

    std::vector<int>           vCliqueVertices;
    std::vector<int>           vOtherRemoved;
    std::vector<std::pair<int,int>> vEdges;
    RetrieveGraphChanges(vCliqueVertices, vOtherRemoved, vEdges);

////    std::cout << "Moving vertex " << vertex << " to X" << std::endl;
////
////    std::cout << "Returning ";
////    for (int const cliqueVertex : vCliqueVertices) {
////        std::cout << cliqueVertex << " ";
////    }
////    std::cout << " to P." << std::endl;

    isolates.ReplaceAllRemoved(vCliqueVertices);
    isolates.ReplaceAllRemoved(vOtherRemoved);

#ifdef REMOVE_ISOLATES
    // restore graph to contain everything in P.
    RemoveFromAdjacencyList(vEdges); // TODO/DS: this probably isn't right

    for (int const cliqueVertex : vCliqueVertices) {
        partialClique.pop_back();
    }
    isolates.RemoveVertex(vertex);
#else
    partialClique.pop_back();
#endif
    m_lDelineators.pop_back();

////    std::cout << __LINE__ << ": " << CheckP() << std::endl;

#if 1 // TODO/DS: Put back!
    // the location of vertex may have changed
    int const vertexLocation = vertexLookup[vertex];

    //swap vertex into X and increment beginP and beginR
    vertexSets[vertexLocation] = vertexSets[beginP];
    vertexLookup[vertexSets[beginP]] = vertexLocation;
    vertexSets[beginP] = vertex;
    vertexLookup[vertex] = beginP;
    beginP = beginP + 1;
////    beginR = beginR + 1; // might need to put this back...

#endif

////    std::cout << __LINE__ << ": " << CheckP() << std::endl;
}

/*! \brief Computes the vertex v in P union X that has the most neighbors in P,
  and places P \ {neighborhood of v} in an array. These are the 
  vertices to consider adding to the partial clique during the current
  recursive call of the algorithm.

  \param pivotNonNeighbors  An intially empty vector, which will contain the set 
  P \ {neighborhood of v} when this function completes.
 */

#define NOT_DONE

// TODO/DS: Choose pivot to maximize number of non-neighbors.
inline std::vector<int> IndependentSetsReduction::ChoosePivot() const
{
////    std::cout << "Size of P: " << beginR - beginP << std::endl;

#ifdef NOT_DONE
    std::vector<int> vVerticesInP;
    for (int i = beginP; i < beginR; ++i) {
        vVerticesInP.push_back(vertexSets[i]);
    }

#ifdef DEBUG
    std::cout << " P : ";
    for (int const neighbor: vVerticesInP)
        std::cout << neighbor << " ";
    std::cout << std::endl;
#endif //DEBUG

    return vVerticesInP;
#else
    int pivot = -1;
    int maxIntersectionSize = -1;
    int i = beginP;

    // loop through all vertices in P union X
    while (i < beginR) {
        int vertex = vertexSets[i];
        int neighborCount = 0;
        int numNeighbors = 0;

        // count the number of neighbors vertex has in P.
        // only count them if the degree of the vertex
        // is greater than the the best count so far.
        int j = 0;
////        if ((beginR - beginP - degree[vertex]) > maxIntersectionSize) {
            // count the number of neighbors vertex has in P.
            while(j < degree[vertex]) {
                int const neighbor = m_AdjacencyList[vertex][j];

                int const neighborLocation = vertexLookup[neighbor];

                if (neighborLocation >= beginP && neighborLocation < beginR)
                    neighborCount++;

                j++;
            }

            // if vertex has more neighbors in P, then update the pivot
            if ((beginR - beginP - neighborCount) > maxIntersectionSize) {
                maxIntersectionSize = beginR - beginP - neighborCount;
                pivot = vertexSets[i];
            }
////        }

        i++;
    }

    // compute non neighbors of pivot by marking its neighbors
    // and moving non-marked vertices into pivotNonNeighbors.
    // we must do this because this is an efficient way
    // to compute non-neighbors of a vertex in 
    // an adjacency list.

    // we initialize enough space for all of P; this is
    // slightly space inefficient, but it results in faster
    // computation of non-neighbors.
    std::vector<int> pivotNonNeighbors;

    std::set<int> verticesToConsider;

    // mark neighbors of pivot that are in P.
    for (int const neighbor : m_AdjacencyList[pivot]) {
        int const neighborLocation = vertexLookup[neighbor];

        // if the neighbor is in P, put it in pivot nonNeighbors
        if (neighborLocation >= beginP && neighborLocation < beginR) {
            verticesToConsider.insert(neighbor);
////            pivotNonNeighbors.push_back(neighbor);
////            for (int nNeighbor : m_AdjacencyList[neighbor]) {
////                int const nNeighborLocation = vertexLookup[nNeighbor];
////                if (nNeighborLocation >= beginP && nNeighborLocation < beginR) {
////                    verticesToConsider.insert(nNeighbor);
////                }
////            }
        }
    }

    pivotNonNeighbors.insert(pivotNonNeighbors.end(), verticesToConsider.begin(), verticesToConsider.end());

////    int const pivotLocation(vertexLookup[pivot]);
////    if (pivotLocation >= beginP && pivotLocation < beginR)
        pivotNonNeighbors.push_back(pivot);

#ifdef DEBUG
    std::cout << " - : ";
    for (int const neighbor: pivotNonNeighbors)
        std::cout << neighbor << " ";
    std::cout << std::endl;
#endif // DEBUG

    return pivotNonNeighbors;
#endif // NOT_DONE
}

inline bool IndependentSetsReduction::InP(int const vertex) const
{
    int const vertexLocation(vertexLookup[vertex]);
    return (vertexLocation >= beginP && vertexLocation < beginR);
}

inline bool IndependentSetsReduction::PIsEmpty() const
{
    return (beginP >= beginR);
}

inline bool IndependentSetsReduction::XAndPAreEmpty() const
{
    return (beginX >= beginP && beginP >= beginR);
}

#endif //INDEPENDENT_SETS_REDUCTION_H
