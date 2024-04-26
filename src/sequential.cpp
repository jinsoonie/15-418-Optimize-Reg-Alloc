/***
 *
 * Sequential Coloring Version using Greedy Algorithm, uses InterferenceGraph class
 *
 * ***/

#include "graph.h"
#include <iostream>
#include <cstdlib>
#include <set>
#include <unordered_map>

class seqColorGraph : public InterferenceGraph {
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
        // Now based on the ordering, we can start coloring the graph
        // While the ordering is not empty, we will color the nodes
        while (!ordering.empty()) {
            int currentNode = ordering.back();
            ordering.pop_back();

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

        // // Start coloring each vertex starting from node 0 to numNodes-1
        // for (int currentNode = 0; currentNode < numNodes; currentNode++) {
        //     std::set<int> usedColors;

        //     // Find all colors used by adjacent vertices (neighborhood N(v))
        //     // improved so now only takes ~0.54 sec to run 5000 v / 2.5mil edges
        //     for (int neighbor : adjList[currentNode]) {
        //         if (nodeColors[neighbor] != -1) {
        //             usedColors.insert(nodeColors[neighbor]);
        //         }
        //     }

        //     // Find the lowest unused color in N(v)
        //     int color = 0;
        //     while (usedColors.find(color) != usedColors.end()) {
        //         color++;
        //     }

        //     // Assign the found color to the current node
        //     nodeColors[currentNode] = color;
        // }
    }
};
