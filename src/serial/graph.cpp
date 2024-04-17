// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>, Rex Kim <rexk@andrew.cmu.edu>

// This file contains the implementation of the Interference Graph Generator
// The purpose of this generator is to generate graphs that we'll feed into
// our graph search and coloring algorithms.

#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>

// added nodeColors to InterferenceGraph
InterferenceGraph::InterferenceGraph(int nodes, int edges) : numNodes(nodes), numEdges(edges), nodeColors(nodes, -1), adjList(nodes) {
    // generate a random seed
    srand(static_cast<unsigned int>(time(nullptr)));
}

// --WARNING--: This takes A LONG TIME TO RUN when there are a lot of edges! run once and beware
void InterferenceGraph::generateGraph() {
    // initialize an edgeset
    std::set<std::pair<int, int> > edgesSet;

    // check we don't have specified number of edges
    while (edgesSet.size() < static_cast<size_t>(numEdges)) {
        // generate random nodes
        int u = rand() % numNodes;
        int v = rand() % numNodes;

        // check if the edge already exists
        bool found = (edgesSet.find({ u, v }) != edgesSet.end()) || (edgesSet.find({ v, u }) != edgesSet.end());
        if (u != v && !found) {
            edgesSet.insert({ u, v });

            // generate adjacency list using allEdges
            adjList[u].push_back(v);
            adjList[v].push_back(u);
        }
    }

    allEdges.assign(edgesSet.begin(), edgesSet.end());
}

void InterferenceGraph::printGraph() {
    std::cout << "Interference Graph" << std::endl;
    for (const auto& edge : allEdges) {
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

// ADDED verifyColoring (check that graph coloring is indeed valid)
bool InterferenceGraph::verifyColoring() const {
    // First check if all nodes are colored
    for (int color : nodeColors) {
        if (color == -1) {
            std::cout << "Coloring error: Not all nodes are colored. Found default val -1 color" << std::endl;
            return false;
        }
    }

    // Check for any coloring errors between adjacent nodes
    /* for (const auto& edge : allEdges) {
        int u = edge.first;
        int v = edge.second;
        if (nodeColors[u] == nodeColors[v]) {
            std::cout << "Coloring error: Node " << u << " and Node " << v << " have the same color " << nodeColors[u] << std::endl;
            return false;
        }
    } */
    for (int currentNode = 0; currentNode < numNodes; currentNode++) {
        for (int neighbor : adjList[currentNode]) {
            if (nodeColors[neighbor] == nodeColors[currentNode]) {
                std::cout << "Coloring error: Node " << currentNode << " and Node " << neighbor << " have the same color " << nodeColors[currentNode] << std::endl;
                return false;
            }
        }
    }

    return true;
}

// ADDED count # of colors used in graph, use a set bc unique colors
int InterferenceGraph::countUniqueColors() const {
    std::set<int> uniqueColors;
    for (int color : nodeColors) {
        if (color != -1) { // Assuming -1 means uncolored
            uniqueColors.insert(color);
        }
    }
    return uniqueColors.size();
}

// ADDED: GreedyColoring for graph class (each child class will have its own version overwrite this)
void InterferenceGraph::greedyColoring() {
    std::cout << "From greedyColoring(): Default greedyColoring for InterferenceGraph should do nothing\n";
}
