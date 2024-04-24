/***
 *
 * Sequential Coloring Version using Greedy Algorithm, uses InterferenceGraph class
 *
 * ***/

#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <set>

class seqColorGraph : public InterferenceGraph {
public:
    // child class of InterferenceGraph, use same constructor 
    using InterferenceGraph::InterferenceGraph;

    void greedyColoring() {
        // Start coloring each vertex starting from node 0 to numNodes-1
        for (int currentNode = 0; currentNode < numNodes; currentNode++) {
            std::set<int> usedColors;

            // Find all colors used by adjacent vertices (neighborhood N(v))
            // improved so now only takes ~0.54 sec to run 5000 v / 2.5mil edges
            for (int neighbor : adjList[currentNode]) {
                if (nodeColors[neighbor] != -1) {
                    usedColors.insert(nodeColors[neighbor]);
                }
            }

            // Find the lowest unused color in N(v)
            int color = 0;
            while (usedColors.find(color) != usedColors.end()) {
                color++;
            }

            // Assign the found color to the current node
            nodeColors[currentNode] = color;
        }
    }
};
