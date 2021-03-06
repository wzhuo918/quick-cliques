#include "IsolatesIndependentSetColoringStrategy.h"
#include "DegeneracyTools.h"
#include "Isolates3.h"
#include "Isolates4.h"
#include "IsolatesWithMatrix.h"
#include "ArraySet.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

template <typename IsolatesType>
IsolatesIndependentSetColoringStrategy<IsolatesType>::IsolatesIndependentSetColoringStrategy(IsolatesType const &isolates, size_t const numVerticesInOriginalGraph)
 : ColoringStrategy()
 , m_vVertexToColor(numVerticesInOriginalGraph, -1)
 , m_vvVerticesWithColor(numVerticesInOriginalGraph)
 , m_vNeighborColorCount(numVerticesInOriginalGraph, 0)
 , m_vbNeighbors(numVerticesInOriginalGraph, false)
 , m_vbConflictNeighbors(numVerticesInOriginalGraph, false)
 , m_Isolates(isolates)
 , m_ColorTimer(0)
{
}

template <typename IsolatesType>
IsolatesIndependentSetColoringStrategy<IsolatesType>::~IsolatesIndependentSetColoringStrategy()
{
////    cout << "Total time to perform coloring: " << Tools::GetTimeInSeconds(m_ColorTimer) << endl << flush;
}

template <typename IsolatesType>
void IsolatesIndependentSetColoringStrategy<IsolatesType>::Color(IsolatesType const &isolates, vector<int> const &vVertexOrder, vector<int> &vVerticesToReorder, vector<int> &vColors)
{
    if (vVerticesToReorder.empty()) return;

#if 0
    cout << "Coloring (in ): ";
////    for (int const vertex : vVerticesToReorder) {
    for (int index = 0; index < vVerticesToReorder.size(); index++) {
        int const vertex(vVerticesToReorder[index]);
        cout << vertex << "(" << vColors[index] << ") ";
    }
    cout << endl;
#endif // 0

    int maxColor(-1);

    size_t const numVerticesToReorder(vVerticesToReorder.size());

    for (int const vertex : vVertexOrder) {

        // choose the smallest color of any non-neighbor vertex.
#if 1
        int uSmallestFreeColor = maxColor + 1;
        // first count the number of neighbors with a given color
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            if (m_vVertexToColor[neighbor] != -1) {
                m_vNeighborColorCount[m_vVertexToColor[neighbor]]++;
            }
        }

        // compare color counts to total number of vertices with the color
        // if there is a difference, then there exists a non-neighbor with
        // that color; otherwise the color is free. Pick the smallest such
        // free color
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            int const currentColor(m_vVertexToColor[neighbor]);
            if (currentColor != -1 && static_cast<int>(m_vvVerticesWithColor[currentColor].size()) == m_vNeighborColorCount[currentColor]) {
                uSmallestFreeColor = min(currentColor, uSmallestFreeColor);
            }
        }

        // put color counts back to 0.
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            if (m_vVertexToColor[neighbor] != -1) {
                m_vNeighborColorCount[m_vVertexToColor[neighbor]] = 0;
            }
        }
#else
        int uSmallestFreeColor(0);
        for (vector<int> const &verticesWithColor : m_vvVerticesWithColor) {
            bool hasNeighborWithColor(false);
            if (verticesWithColor.empty()) break;
            for (int const coloredVertex : verticesWithColor) {
                // can be made more efficient?
                hasNeighborWithColor = (find(adjacencyArray[vertex].begin(), adjacencyArray[vertex].end(), coloredVertex) == adjacencyArray[vertex].end());
                if (hasNeighborWithColor) {
                    uSmallestFreeColor++;
                    break;
                }
            }

            if (!hasNeighborWithColor) {
                break;
            }
        }
#endif // 1

        m_vvVerticesWithColor[uSmallestFreeColor].push_back(vertex);
        m_vVertexToColor[vertex] = uSmallestFreeColor;
        maxColor = max(maxColor, uSmallestFreeColor);
    }

////    cout << "maxColor=" << maxColor << ", numVertices=" << vVerticesToReorder.size() << endl;

    int currentIndex(0);
    int currentColor(0);
    for (int currentColor = 0; currentColor <= maxColor; ++currentColor) {
        for (int const vertex : m_vvVerticesWithColor[currentColor]) {
            vVerticesToReorder[currentIndex] = vertex;
            vColors[currentIndex] = currentColor+1;
            currentIndex++;
            m_vVertexToColor[vertex] = -1;
        }
        m_vvVerticesWithColor[currentColor].clear();
    }

#if 0
    cout << "Coloring (out): ";
    for (int index = 0; index < vVerticesToReorder.size(); index++) {
        int const vertex(vVerticesToReorder[index]);
        cout << vertex << "(" << vColors[index] << ") ";
    }
    cout << endl;
#endif // 0

// verify that it is a valid coloring.
#ifdef DEBUG
    vector<int> vColor(adjacencyList.size(), -1);
    for (size_t index = 0; index < vVerticesToReorder.size(); ++index) {
        vColor[vVerticesToReorder[index]] = vColors[index];
    }
    for (int const vertex : vVerticesToReorder) {
        for (int const neighbor : adjacencyList[vertex]) {
            if (vColor[vertex] == vColor[neighbor]) {
                cout << "Consistency Error: vertex " << vertex << " has the same color as neighbor " << neighbor << ", color=" << vColor[vertex] << endl << flush;
            }
        }
    }
#endif // DEBUG
}


// TODO/DS: Speedup recolor in sparse framework
template <typename IsolatesType>
void IsolatesIndependentSetColoringStrategy<IsolatesType>::Recolor(IsolatesType const &isolates,  vector<int> const &vVertexOrder, vector<int> &vVerticesToReorder, vector<int> &vColors, int const currentBestCliqueSize, int const currentCliqueSize)
{
    if (vVerticesToReorder.empty()) return;

////    clock_t const start(clock());

#if 0
    cout << "Coloring (in ): ";
////    for (int const vertex : vVerticesToReorder) {
    for (int index = 0; index < vVerticesToReorder.size(); index++) {
        int const vertex(vVerticesToReorder[index]);
        cout << vertex << "(" << vColors[index] << ") ";
    }
    cout << endl;
#endif // 0

    int maxColor(-1);

    int iBestCliqueDelta(currentBestCliqueSize - currentCliqueSize);

    size_t const numVerticesToReorder(vVerticesToReorder.size());

    for (int const vertex : vVertexOrder) {
////        bool debug(vertex==29);
////
////        if (debug) {
////        cout << "State: " << endl;
////        for (size_t colorIndex=0; colorIndex < m_vvVerticesWithColor.size(); colorIndex++) {
////            cout << "    Color " << colorIndex << ": ";
////            if (m_vvVerticesWithColor[colorIndex].empty()) break;
////            for (int const vertex : m_vvVerticesWithColor[colorIndex]) {
////                cout << vertex << " ";
////            }
////            cout << endl;
////        }
////        cout << endl;
////        cout << "Neighbors: " << endl;
////        for (int const neighbor : m_AdjacencyArray[vertex]) {
////            cout << neighbor << " ";
////        }
////        cout << endl;
////        }

#if 1
        int uSmallestFreeColor = maxColor + 1;
        // first count the number of neighbors with a given color
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            m_vbNeighbors[neighbor] = true;
            if (m_vVertexToColor[neighbor] != -1) {
                m_vNeighborColorCount[m_vVertexToColor[neighbor]]++;
            }
        }

        // compare color counts to total number of vertices with the color
        // if there is a difference, then there exists a non-neighbor with
        // that color; otherwise the color is free. Pick the smallest such
        // free color
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            int const currentColor(m_vVertexToColor[neighbor]);
////            if (debug && neighbor==28) {
////                cout << " neighbor 28 has color " << currentColor << ", there are " << m_vvVerticesWithColor[currentColor].size() << " vertices with that color, and " << m_vNeighborColorCount[currentColor] << " neighbors with that color" << endl;
////            }
            if (currentColor != -1 && static_cast<int>(m_vvVerticesWithColor[currentColor].size()) == m_vNeighborColorCount[currentColor]) {
                uSmallestFreeColor = min(currentColor, static_cast<int>(uSmallestFreeColor));
            }
        }

////        // put color counts back to 0.
////        for (int const neighbor : m_AdjacencyArray[vertex]) {
////            if (m_vVertexToColor[neighbor] != -1) {
////                m_vNeighborColorCount[m_vVertexToColor[neighbor]] = 0;
////            }
////        }

#else
        int uSmallestFreeColor = 0;
        for (vector<int> const &verticesWithColor : m_vvVerticesWithColor) {
            bool hasNeighborWithColor(false);
            if (verticesWithColor.empty()) break;
            for (int const coloredVertex : verticesWithColor) {
                // can be made more efficient?
                hasNeighborWithColor = (find(adjacencyArray[vertex].begin(), adjacencyArray[vertex].end(), coloredVertex) == adjacencyArray[vertex].end());
                if (hasNeighborWithColor) {
                    uSmallestFreeColor++;
                    break;
                }
            }

            if (!hasNeighborWithColor) {
                break;
            }
        }
#endif // 1
////        cout << "vertex " << vertex << " gets initial color " << uSmallestFreeColor << endl;

        m_vvVerticesWithColor[uSmallestFreeColor].push_back(vertex);
        m_vVertexToColor[vertex] = uSmallestFreeColor;
        maxColor = max(maxColor, uSmallestFreeColor);
        if (uSmallestFreeColor +1 > iBestCliqueDelta && /*m_vvVerticesWithColor[color].size() == 1*/ uSmallestFreeColor == maxColor) {
            Repair(vertex, uSmallestFreeColor, iBestCliqueDelta);
            if (m_vvVerticesWithColor[maxColor].empty())
                maxColor--;
        }

        // put color counts back to 0. Needs to come after repair, repair uses the counts.
        for (int const neighbor : m_Isolates.Neighbors()[vertex]) {
            m_vbNeighbors[neighbor] = false;
            if (m_vVertexToColor[neighbor] != -1) {
                m_vNeighborColorCount[m_vVertexToColor[neighbor]] = 0;
            }
        }
    }

////    cout << "maxColor=" << maxColor << ", numVertices=" << vVerticesToReorder.size() << endl;

    int currentIndex(0);
    int currentColor(0);
    for (int currentColor = 0; currentColor <= maxColor; ++currentColor) {
        for (int const vertex : m_vvVerticesWithColor[currentColor]) {
            vVerticesToReorder[currentIndex] = vertex;
            vColors[currentIndex] = currentColor+1;
            currentIndex++;

            m_vVertexToColor[vertex] = -1;
        }
        m_vvVerticesWithColor[currentColor].clear();
    }

#if 0
    cout << "Coloring (out): ";
    for (int index = 0; index < vVerticesToReorder.size(); index++) {
        int const vertex(vVerticesToReorder[index]);
        cout << vertex << "(" << vColors[index] << ") ";
    }
    cout << endl;
#endif // 0

// verify that it is a valid coloring.
#ifdef DEBUG
    vector<int> vColor(adjacencyList.size(), -1);
    for (size_t index = 0; index < vVerticesToReorder.size(); ++index) {
        vColor[vVerticesToReorder[index]] = vColors[index];
    }
    for (int const vertex : vVerticesToReorder) {
        for (int const neighbor : adjacencyList[vertex]) {
            if (vColor[vertex] == vColor[neighbor]) {
                cout << "Consistency Error: vertex " << vertex << " has the same color as neighbor " << neighbor << ", color=" << vColor[vertex] << endl << flush;
            }
        }
    }
#endif // DEBUG

////    m_ColorTimer += (clock() - start);
}

template <typename IsolatesType>
bool IsolatesIndependentSetColoringStrategy<IsolatesType>::HasConflict(int const vertex, vector<int> const &vVerticesWithColor)
{
    if (vVerticesWithColor.empty()) return false;
    for (int const coloredVertex : vVerticesWithColor) {
        // can be made more efficient?
        if (!m_vbConflictNeighbors[coloredVertex]) {
            return true;
        }
    }

    return false;
}

template <typename IsolatesType>
int IsolatesIndependentSetColoringStrategy<IsolatesType>::GetConflictingVertex(int const vertex, vector<int> const &vVerticesWithColor)
{
    int conflictingVertex(-1);
    int count(0);
    for (int const candidateVertex : vVerticesWithColor) {
        if (!m_vbNeighbors[candidateVertex]) { ////find(m_AdjacencyArray[vertex].begin(), m_AdjacencyArray[vertex].end(), candidateVertex) == m_AdjacencyArray[vertex].end()) {
            conflictingVertex = candidateVertex;
            count++;
            if (count > 1) return -1;
        }
    }
    return conflictingVertex;
}

template <typename IsolatesType>
bool IsolatesIndependentSetColoringStrategy<IsolatesType>::Repair(int const vertex, int const color, int const iBestCliqueDelta)
{
    for (int newColor = 0; newColor <= iBestCliqueDelta-1; newColor++) {
        int const conflictingVertex(GetConflictingVertex(vertex, m_vvVerticesWithColor[newColor]));
        if (conflictingVertex < 0) continue;

        for (int const neighbor : m_Isolates.Neighbors()[conflictingVertex]) {
            m_vbConflictNeighbors[neighbor] = true;
        }

        // TODO/DS: put conflicting neighbors into array for quick testing.
        for (int nextColor = newColor+1; nextColor <= iBestCliqueDelta; nextColor++) {
            if (HasConflict(conflictingVertex, m_vvVerticesWithColor[nextColor])) continue;
            m_vvVerticesWithColor[color].erase(find(m_vvVerticesWithColor[color].begin(), m_vvVerticesWithColor[color].end(), vertex));
            m_vvVerticesWithColor[newColor].erase(find(m_vvVerticesWithColor[newColor].begin(), m_vvVerticesWithColor[newColor].end(), conflictingVertex));
            m_vvVerticesWithColor[newColor].push_back(vertex);
            m_vvVerticesWithColor[nextColor].push_back(conflictingVertex);

            m_vVertexToColor[vertex] = newColor;
            m_vVertexToColor[conflictingVertex] = nextColor;
////            cout << "Repairing vertices " << vertex << " and " << conflictingVertex << endl;

            for (int const neighbor : m_Isolates.Neighbors()[conflictingVertex]) {
                m_vbConflictNeighbors[neighbor] = false;
            }
            return true;
        }

        for (int const neighbor : m_Isolates.Neighbors()[conflictingVertex]) {
            m_vbConflictNeighbors[neighbor] = false;
        }
    }
    return false;
}

template class IsolatesIndependentSetColoringStrategy<Isolates3<ArraySet>>;
template class IsolatesIndependentSetColoringStrategy<IsolatesWithMatrix<ArraySet>>;
template class IsolatesIndependentSetColoringStrategy<Isolates4<SparseArraySet>>;
template class IsolatesIndependentSetColoringStrategy<Isolates4<ArraySet>>;
