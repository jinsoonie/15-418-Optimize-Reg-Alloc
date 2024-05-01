// 15.418 Final Project: Parallel MCS and Graph Coloring
// Graph Generator
// Author: Kevin Huang <ykhuang@andrew.cmu.edu>, Rex Kim <rexk@andrew.cmu.edu>

// This file contains the implementation of the main driver code 
// for the MCS and Graph Coloring algorithms.


#include "graph.h"
#include <iostream>
#include <cstdlib>

#include "timer.h"
#include <cstring>
#include "sequential.cpp"
#include "openmp-v1.cpp"
#include "openmp-v2.cpp"
#include "openmp-v3.cpp"
#include "openmp-v4.cpp"
#include <memory>


int main(int argc, char* argv[]) {
    // Check for correct number of args
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <numNodes> <numEdges> -[desired mode]" << std::endl;
        return 1;
    }

    /** BEGIN ADDED **/
    std::string mode = "none"; // Default mode when no flags are set
    int modeCount = 0; // Counter to track the number of mode flags set

    // Iterate over all arguments provided (starting from 1 to skip program name)
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-seq") == 0) {
            mode = "Sequential";
            modeCount++;
        }
        else if (strcmp(argv[i], "-openmpv1") == 0) {
            mode = "OpenMPv1";
            modeCount++;
        }
        // openMPv2, better than v1? (roughly x10 faster, but x1.5 colors)
        else if (strcmp(argv[i], "-openmpv2") == 0) {
            mode = "OpenMPv2";
            modeCount++;
        }
        // openMPv3, TESTING MCS PARALLELIZATION
        else if (strcmp(argv[i], "-openmpv3") == 0) {
            mode = "OpenMPv3";
            modeCount++;
        }
        // openMPv4, TESTING Jones-Plassman
        else if (strcmp(argv[i], "-openmpv4") == 0) {
            mode = "OpenMPv4";
            modeCount++;
        }
        else if (strcmp(argv[i], "-openmpi") == 0) {
            mode = "OpenMPI";
            modeCount++;
        }

        // Check if more than one mode has been set
        if (modeCount > 1) {
            std::cerr << "Error: Multiple execution modes specified." << std::endl;
            return 1;
        }
    }

    // Output the active mode if only one is set, otherwise output none
    std::cout << "Active execution mode: " << (modeCount == 1 ? mode : "none") << std::endl;

    /** END ADDED **/

    // Check for valid arguments
    if (std::atoi(argv[1]) <= 0 || std::atoi(argv[2]) <= 0) {
        std::cerr << "Invalid arguments. Please enter positive integers for numNodes and numEdges." << std::endl;
        return 1;
    }

    // Parse command line arguments
    int numNodes = std::atoi(argv[1]); // Example: 10 nodes
    int numEdges = std::atoi(argv[2]); // Example: 15 edges

    std::uint64_t testValid = static_cast<std::uint64_t>(numNodes - 1) * numNodes / 2;

    if (numEdges > testValid) {
        std::cerr << "Invalid arguments. The number of edges must be less than or equal to numNodes * (numNodes - 1) / 2" << std::endl;
        return 1;
    }

    /** BEGIN ADDED **/
    // Generate the graph ACCORDING to which mode selected
    // InterferenceGraph graph(numNodes, numEdges);

    std::unique_ptr<InterferenceGraph> graph;

    if (mode == "Sequential") {
        graph = std::make_unique<seqColorGraph>(numNodes, numEdges);
    }
    else if (mode == "OpenMPv1") {
        // WIP
        graph = std::make_unique<openmpV1ColorGraph>(numNodes, numEdges);
    }
    // openMPv2, better than v1? (roughly x10 faster, but x1.5 colors)
    else if (mode == "OpenMPv2") {
        graph = std::make_unique<openmpV2ColorGraph>(numNodes, numEdges);
    }
    // openMPv3, TESTING MCS PARALLELIZATION
    else if (mode == "OpenMPv3") {
        graph = std::make_unique<openmpV3ColorGraph>(numNodes, numEdges);
    }
    // openMPv4, TESTING Jones-Plassman
    else if (mode == "OpenMPv4") {
        graph = std::make_unique<openmpV4ColorGraph>(numNodes, numEdges);
    }
    else if (mode == "OpenMPI") {
        // WIP
    }
    else {
        graph = std::make_unique<InterferenceGraph>(numNodes, numEdges);
        std::cout << "Error no mode specified, defaulting to InterferenceGraph that does not have any coloring" << std::endl;
    }



    // TIMER instantiation - use this to do timing of program
    Timer programTimer;
    programTimer.reset();

    graph->generateGraph();
    std::cout << "Elapsed time for generating graph: " << programTimer.elapsed() << " seconds\n";

    //graph->printGraph();
    programTimer.reset();
    // run the program here...
    graph->maximumCardinalitySearch();  // Apply MCS
    std::cout << "Elapsed time for MCS: " << programTimer.elapsed() << " seconds\n";

    programTimer.reset();
    // run the program here...
    graph->greedyColoring();  // Apply greedy coloring


    std::cout << "Elapsed time for graph coloring: " << programTimer.elapsed() << " seconds\n";

    std::cout << "Colors used: " << graph->countUniqueColors() << " colors\n";

    // verify graph is colored correctly 
    if (!graph->verifyColoring()) {
        std::cout << "Incorrect: Graph was not colored correctly!\n";
        return 1;
    }

    /** END ADDED **/

    // Now we want to do MCS on the graph


    return 0;
}
