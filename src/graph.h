// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator Header File
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>, Rex Kim <rexk@andrew.cmu.edu>

// This file contains the class definition for the Interference Graph Generator


#ifndef INTERFERENCE_GRAPH_H
#define INTERFERENCE_GRAPH_H

#include <vector>
#include <utility>

class InterferenceGraph {
public:
    InterferenceGraph(int nodes, int edges);
    void generateGraph();
    void printGraph();
    // ADDED
    bool verifyColoring() const;
    int countUniqueColors() const;
    virtual void maximumCardinalitySearch(); // virtual bc will be overridden by children
    virtual void greedyColoring();  // virtual bc will be overridden by children
    virtual ~InterferenceGraph() {}; // virtual destructor or else undef behavior when children access

    // changed to protected so children classes can access and change nodeColors
protected:
    int numNodes;
    int numEdges;
    int numColors;
    // ADDED
    std::vector< std::pair<int, int> > allEdges; // Vector to store edges as pairs
    std::vector<int> nodeColors;                 // Colors assigned to each node
    std::vector<std::vector<int>> adjList;
    std::vector<int> ordering;

};

#endif