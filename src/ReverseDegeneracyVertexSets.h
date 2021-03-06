#ifndef _DJS_REVERSE_DEGENERACY_VERTEX_SETS_H_
#define _DJS_REVERSE_DEGENERACY_VERTEX_SETS_H_

/* 
    This program is free software: you can redistribute it and/or modify 
    it under the terms of the GNU General Public License as published by 
    the Free Software Foundation, either version 3 of the License, or 
    (at your option) any later version. 
 
    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details. 
 
    You should have received a copy of the GNU General Public License 
    along with this program.  If not, see <http://www.gnu.org/licenses/> 
*/

// local includes
#include "DegeneracyVertexSets.h"
#include "DegeneracyTools.h"

// system includes
#include <list>
#include <vector>
#include <cstring> // memcpy
#include <algorithm>

/*! \file FasterDegeneracyAlgorithm.h

    \brief see FasterDegeneracyAlgorithm.cpp

    \author Darren Strash (first name DOT last name AT gmail DOT com)

    \copyright Copyright (c) 2011 Darren Strash. This code is released under the GNU Public License (GPL) 3.0.

    \image html gplv3-127x51.png

    \htmlonly
    <center>
    <a href="gpl-3.0-standalone.html">See GPL 3.0 here</a>
    </center>
    \endhtmlonly
*/

class ReverseDegeneracyVertexSets : public DegeneracyVertexSets {
public:
    ReverseDegeneracyVertexSets(std::vector<std::vector<int>> &adjacencyList);
    ~ReverseDegeneracyVertexSets();

    ReverseDegeneracyVertexSets           (ReverseDegeneracyVertexSets const &sets) = delete;
    ReverseDegeneracyVertexSets& operator=(ReverseDegeneracyVertexSets const &sets) = delete;

    virtual bool GetNextTopLevelPartition();

    virtual void GetTopLevelPartialClique(std::list<int> &partialClique);

protected:
    size_t m_uCurrentEarlierIndex;
};

#endif //_DJS_REVERSE_DEGENERACY_ALGORITHM_H_
