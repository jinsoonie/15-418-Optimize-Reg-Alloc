/***
 *
 * OpenMP Coloring Version 2 using Greedy Algorithm, uses InterferenceGraph class
 * vs. Version 1: (ROUGHLY x6.7 FASTER, BUT USES x1.6 COLORS)
 * on a 5000 node, 2500000 edge random generated, ~0.08s / ~315 colors
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

class openmpV2ColorGraph : public InterferenceGraph {
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
        // std::cout << "Ordering: ";
        // for (int node : ordering) {
        //     std::cout << node << " ";
        // }
        // std::cout << std::endl; //
    }

    void greedyColoring() {
        omp_set_num_threads(numThreads);  // Set the number of threads

        // first, color all in parallel (will be race conditions)
        // access "ordering" in for loop so can use openMP parallel for
#pragma omp parallel for shared(adjList, nodeColors)
        for (int i = 0; i < numNodes; i++) {
            int currentNode = ordering[i];

            // Find all colors used by adjacent vertices (neighborhood N(v))
            std::set<int> usedColors;
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

        // find the current (most likely incorrect) total # of colors
        int totalColors = 0;
        for (int i = 0; i < numNodes; i++) {
            totalColors = std::max(totalColors, nodeColors[i] + 1);
        }

        // loop through in parallel, if conflict detected, then resolve by using totalColors+1
#pragma omp parallel for shared(adjList, nodeColors, totalColors)
        for (int i = 0; i < numNodes; i++) {
            int currentNode = ordering[i];
            int currNodeColor = nodeColors[currentNode];

            for (int neighbor : adjList[currentNode]) {
                // conflict detected, should resolve atomically by using totalColors += 1
                if (currNodeColor == nodeColors[neighbor]) {
#pragma omp atomic capture
                    nodeColors[currentNode] = totalColors++;
                    break;
                }
            }
        }
    }
};
