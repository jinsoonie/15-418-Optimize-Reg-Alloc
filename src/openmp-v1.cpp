/***
 *
 * OpenMP Coloring Version 1 using Greedy Algorithm, uses InterferenceGraph class
 *
 * ***/
#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <set>
#include <omp.h>
#include <vector>
#include <set>

class openmpV1ColorGraph : public InterferenceGraph {
public:
    // child class of InterferenceGraph, use same constructor 
    using InterferenceGraph::InterferenceGraph;

    void greedyColoring() {
        // Parallelizing the loop might not be straightforward because each iteration is not independent
        for (int currentNode = 0; currentNode < numNodes; currentNode++) {
            std::set<int> usedColors;

            // Collect used colors in parallel
#pragma omp parallel
            {
                std::set<int> localUsedColors;

#pragma omp for nowait // nowait allows to skip the implicit barrier at the end of the for
                for (int i = 0; i < adjList[currentNode].size(); i++) {
                    int neighbor = adjList[currentNode][i];
                    if (nodeColors[neighbor] != -1) {
                        localUsedColors.insert(nodeColors[neighbor]);
                    }
                }

                // Critical section to merge local sets into the global set
#pragma omp critical
                {
                    usedColors.insert(localUsedColors.begin(), localUsedColors.end());
                }
            }

            // Find the lowest unused color
            int color = 0;
            while (usedColors.find(color) != usedColors.end()) {
                color++;
            }

            // Assign the found color to the current node
            // This part must remain sequential because it depends directly on the result of the usedColors calculation
            nodeColors[currentNode] = color;
        }
    }
};
