/***
 *
 * OpenMP Coloring Version 4 using Jones-Plassman Algorithm, uses InterferenceGraph class
 * slower than openMPv1 (Greedy) but has better coloring quality (not as worth tradeoff, however)
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
#include <algorithm>

class openmpV4ColorGraph : public InterferenceGraph {
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

    // JONES-PLASSMAN (parallel) - slower than openMPV1 but has better coloring quality
    // refer to https://users.cs.utah.edu/~hari/teaching/bigdata/06_Randomized_Algorithms.pdf
    void greedyColoring() {
        // vector to show how many nodes remaining
        std::vector<bool> remaining(numNodes, true);

        while (std::find(remaining.begin(), remaining.end(), true) != remaining.end()) {
            std::vector<int> independentSet;

            // Finding an independent set in parallel
#pragma omp parallel for
            for (int currentNode = 0; currentNode < numNodes; currentNode++) {
                // only consider nodes that are uncolored
                if (remaining[currentNode]) {
                    bool isHighest = true;
                    for (int neighbor : adjList[currentNode]) {
                        // if neighbor is greater priority and neighbor is uncolored
                        if (neighbor > currentNode && remaining[neighbor]) {
                            isHighest = false;
                            break;
                        }
                    }
                    // according to JP, if curr node is highest priority, add to coloring set
                    if (isHighest) {
#pragma omp critical
                        independentSet.push_back(currentNode);
                    }
                }
            }

            // Coloring the vertices in the independent set in parallel
#pragma omp parallel for
            for (auto currentNode : independentSet) {
                std::set<int> usedColors;
                for (int neighbor : adjList[currentNode]) {
                    if (nodeColors[neighbor] != -1) {
                        usedColors.insert(nodeColors[neighbor]);
                    }
                }

                // Assign the lowest available color
                int color = 0;
                while (usedColors.find(color) != usedColors.end()) {
                    color++;
                }
                nodeColors[currentNode] = color;
                remaining[currentNode] = false;
            }
        }
    }
};
