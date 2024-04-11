#include "graph.h"

int main() {
    int numNodes = 10; // Example: 10 nodes
    int numEdges = 15; // Example: 15 edges

    InterferenceGraph graph(numNodes, numEdges);
    graph.generateGraph();
    graph.printGraph();

    return 0;
}
