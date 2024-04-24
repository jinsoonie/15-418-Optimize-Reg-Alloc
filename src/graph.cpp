// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>

// This file contains the implementation of the Interference Graph Generator
// The purpose of this generator is to generate graphs that we'll feed into
// our graph search and coloring algorithms.

#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>

InterferenceGraph::InterferenceGraph(int nodes, int edges) : numNodes(nodes), numEdges(edges) {
    // generate a random seed
    srand(static_cast<unsigned int>(time(nullptr)));
}

void InterferenceGraph::generateGraph() {
    // initialize an edgeset
    std::set<std::pair<int, int> > edgesSet;

    // check we don't have specified number of edges
    while(edgesSet.size() < static_cast<size_t>(numEdges)) {
        // generate random nodes
        int u = rand() % numNodes;
        int v = rand() % numNodes;

        // check if the edge already exists
        bool found = (edgesSet.find({u, v}) != edgesSet.end()) || (edgesSet.find({v, u}) != edgesSet.end());
        if (u != v && !found) {
            edgesSet.insert({u, v});
        }
    }

    adjacencyList.assign(edgesSet.begin(), edgesSet.end());
}

void InterferenceGraph::printGraph() const {
    std::cout << "Interference Graph" << std::endl;
    for (const auto& edge : adjacencyList) {
        std::cout << "Edge from Node " << edge.first << " to Node " << edge.second << std::endl;
    }
}

std::vector<int> InterferenceGraph::maximumCardinalitySearch() const {
    std::vector<int> ordering(numNodes, 0);
    std::vector<int> weights(numNodes, 0);
    std::vector<bool> removed(numNodes, false);

    /* TODO: Implement MCS */
    return ordering;
}
