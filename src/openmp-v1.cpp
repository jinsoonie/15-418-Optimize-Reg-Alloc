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
#include <unordered_map>

class openmpV1ColorGraph : public InterferenceGraph {
public:
    // child class of InterferenceGraph, use same constructor 
    using InterferenceGraph::InterferenceGraph;

    /* This implementation is a serial version of the maximum cardinality search algorithm */
    void maximumCardinalitySearch() {
        // Define the weight hashmap
        std::unordered_map<int, int> weights;   
        
        // Initialize all weights to 1
        for (int i = 0; i < numNodes; i++) {
            weights[i] = 1;
        }

        // Here we start the search
        for (int i = 0; i < numNodes; i++) {
            int maxDegree = -1;
            int maxNode = -1;

            // Go through all weights
            for (auto& weight : weights) {
                if (weight.second > maxDegree) {
                    maxDegree = weight.second;
                    maxNode = weight.first;
                }
            }

            // Now that we've chosen a node, we do three things
            // 1. Update the weights of all neighbors
            for (int neighbor : adjList[maxNode]) {
                // if the neighbor is in the weights, increment the weight
                if (weights.find(neighbor) != weights.end()) {
                    weights[neighbor]++;
                }
            }

            // 2. Remove the node from the weights
            weights.erase(maxNode);

            // 3. Push the node into the ordering
            ordering.push_back(maxNode);
        }

        // print the ordering
        std::cout << "Ordering: ";
        for (int node : ordering) {
            std::cout << node << " ";
        }
        std::cout << std::endl;
    }

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
