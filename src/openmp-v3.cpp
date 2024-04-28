/***
 *
 * OpenMP Coloring Version 3 using Greedy Algorithm, uses InterferenceGraph class
 * vs. Version 2: (TESTING PARALLELIZATION OF MCS!! Greedy Algo from v2 untouched)
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

class openmpV3ColorGraph : public InterferenceGraph {
public:
    // child class of InterferenceGraph, use same constructor 
    using InterferenceGraph::InterferenceGraph;

    /* This implementation is a serial version of the maximum cardinality search algorithm */
    void maximumCardinalitySearch() {
        // Define the weight hashmap
        // std::unordered_map<int, int> weights;

        // // Initialize all weights to 1
        // for (int i = 0; i < numNodes; i++) {
        //     weights[i] = 1;
        // }

        // Here we start the search
        // ** CONSIDER USING A PARALLEL REDUCTION HERE **
//         for (int i = 0; i < numNodes; i++) {
//             int maxDegree = -1;
//             int maxNode = -1;

//             // Go through all weights
//             for (auto& weight : weights) {
//                 if (weight.second > maxDegree) {
//                     maxDegree = weight.second;
//                     maxNode = weight.first;
//                 }
//             }
//             // Now that we've chosen a node, we do three things
//             // 1. Update the weights of all neighbors
// #pragma omp parallel for
//             for (int neighbor : adjList[maxNode]) {
//                 // if the neighbor is in the weights, increment the weight
// //#pragma omp critical
//                 if (weights.find(neighbor) != weights.end()) {
// #pragma omp atomic
//                     weights[neighbor]++;
//                 }
//             }

//             // 2. Remove the node from the weights
//             weights.erase(maxNode);

//             // 3. Push the node into the ordering
//             ordering.push_back(maxNode);
//         }



        // VERSION ABOVE WORKS, BUT IS A BIT SLOW
        // TRY PARALLEL REDUCTION VERSION BELOW

        // vector for weights bc unordered_map for pragma parallel for is weird
        std::vector<int> weights(numNodes, 1);

        for (int i = 0; i < numNodes; i++) {
            int maxDegree = -1;
            int maxNode = -1;

            // parallel reduction to find the node with the maximum weight
#pragma omp parallel
            {
                int localMaxDegree = -1;
                int localMaxNode = -1;

                // each thread finds a local maximum, which is
#pragma omp for nowait
                for (int j = 0; j < numNodes; j++) {
                    if (weights[j] > localMaxDegree) {
                        localMaxDegree = weights[j];
                        localMaxNode = j;
                    }
                }
                // then combined in critical section
#pragma omp critical
                {
                    if (localMaxDegree > maxDegree) {
                        maxDegree = localMaxDegree;
                        maxNode = localMaxNode;
                    }
                }
            }

            // update the weights of all neighbors
#pragma omp parallel for
            for (int neighbor : adjList[maxNode]) {
#pragma omp atomic
                weights[neighbor]++;
            }

            // push the node into the ordering
            ordering.push_back(maxNode);

            // Note: Erasing maxNode from weights not necessary since we won't revisit
            weights[maxNode] = -1;  // Mark this node as used by setting its weight to an invalid state
        }


        // print the ordering
        // std::cout << "Ordering: ";
        // for (int node : ordering) {
        //     std::cout << node << " ";
        // }
        // std::cout << std::endl; //
    }

    void greedyColoring() {
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
