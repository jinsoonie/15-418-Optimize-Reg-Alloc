// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>

// This file contains the implementation of the main driver code 
// for the MCS and Graph Coloring algorithms.


#include "graph.h"
#include <iostream>
#include <cstdlib>


int main(int argc, char *argv[]) {
    // Check for correct number of args
    if (argc != 3){
        std::cerr << "Usage: " << argv[0] << " <numNodes> <numEdges>" << std::endl;
        return 1;
    }

    // Check for valid arguments
    if (std::atoi(argv[1]) <= 0 || std::atoi(argv[2]) <= 0) {
        std::cerr << "Invalid arguments. Please enter positive integers for numNodes and numEdges." << std::endl;
        return 1;
    }

    // Parse command line arguments
    int numNodes = std::atoi(argv[1]); // Example: 10 nodes
    int numEdges = std::atoi(argv[2]); // Example: 15 edges

    if (numEdges > (numNodes - 1)*numNodes) {
        std::cerr << "Invalid arguments. The number of edges must be less than or equal to numNodes * (numNodes - 1)" << std::endl;
        return 1;
    }
    // Generate the graph
    InterferenceGraph graph(numNodes, numEdges);
    graph.generateGraph();
    // graph.printGraph();
    
    // Now we want to do MCS on the graph


    return 0;
}
