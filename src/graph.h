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
    void outputGraph(const char* filename) const;

private:
    int numNodes;
    int numEdges;
    std::vector<std::pair<int, int>> adjacencyList;
};

#endif
