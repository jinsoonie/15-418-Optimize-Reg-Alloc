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
            // THIS IS CAUSING SO MUCH SLOWNESS 
            // (ideally want to store adj list, and iterate for that one node's neighbors, instead of looping thru allEdge each time)
            for (const auto& edge : allEdges) {
                if (edge.first == currentNode && nodeColors[edge.second] != -1) {
                    usedColors.insert(nodeColors[edge.second]);
                }
                if (edge.second == currentNode && nodeColors[edge.first] != -1) {
                    usedColors.insert(nodeColors[edge.first]);
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
