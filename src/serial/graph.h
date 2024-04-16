// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator Header File
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>

// This file contains the class definition for the Interference Graph Generator


#ifndef INTERFERENCE_GRAPH_H
#define INTERFERENCE_GRAPH_H

#include <vector>
#include <utility>

class InterferenceGraph {
public:
    InterferenceGraph(int nodes, int edges);
    void generateGraph();
    void printGraph() const;
    std::vector<int> maximumCardinalitySearch() const;
    // ADDED
    bool verifyColoring() const;
    int countUniqueColors() const;

private:
    int numNodes;
    int numEdges;
    // ADDED
    std::vector< std::pair<int, int> > allEdges; // Vector to store edges as pairs
    std::vector<int> nodeColors;                 // Colors assigned to each node
};

#endif
