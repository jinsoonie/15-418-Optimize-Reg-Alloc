# Parallelization of Register Allocation for Compiler Optimization
by Kevin Huang and Rex Kim

### Summary
We are going to implement an optimized register allocation algorithm in the compiler pipeline on multi-core CPU platforms, and perform a detailed analysis comparing our performance to a sequential baseline implementation.

## Background
Register allocation serves as an important optimization for a compiler. Without proper register allocation, a program would likely be spilling to the stack significantly more than what is necessary, resulting in undesirable performance.

Register allocation starts off with an interference graph generated by the compiler and then goes through two steps: maximum cardinality search (for simplicial ordering) and greedy coloring. These two steps show potential for optimization through parallelization, particularly on larger graphs (generated by large programs).

### Maximum Cardinality Search (MCS)
Maximum cardinality search is typically done in accordance with the following pseudocode.
source: https://www.cs.cmu.edu/~janh/courses/411/24/lectures/03-regalloc.pdf

	Algorithm: MCS
	Input: G = (V, E) with |V| = n
	Output: A simplicial elimination ordering v1,...,vn
	For all v ∈ V set wt(v) ← 0	note that (wt(v) means weight v)
	Let W ← V
	For i ← 1 to n do
		Let v be a node of maximal weight in W
		Set vi ← v
		For all u ∈ W ∩ N(v) set wt(u) ← wt(u) + 1
		Set W ← W \ {v}

### Greedy Coloring
After MCS, we can simply follow the ordering generated by the MCS algorithm and color the graph accordingly. Here is an example pseudocode for the algorithm.
source: https://www.cs.cmu.edu/~janh/courses/411/24/lectures/03-regalloc.pdf

	Algorithm: Greedy coloring
	Input: G = (V, E) and ordered sequence v1,...,vn of nodes
	Output: Assignment col: V → {0, …, ∆(G)}
	For i ← 1 to n do
		Let c be the lowest color not used in N(vi)
		Set col(vi) ← c

### Avenues for Parallelization
Preliminarily we have several ideas on how this algorithm can be parallelized. As there are two parts to this register allocation algorithm, there are two parts that we can try to optimize.

##### Parallelization on MCS
While MCS is inherently sequential, there are several things that we could do to “parallelize” the algorithm.

* When we update the cardinality of each neighbor, we can perform all updates in parallel. On very large graphs, this could prove to be very helpful.

* When selecting the next vertex for MCS we could search through all vertices in parallel using atomic operations here to search with vertex with the largest weight.

##### Parallelization on Greedy Coloring
Our greedy coloring algorithm is a little different from the typical k-coloring problem as we generate an ordering to color our nodes with MCS. There are still ways we can parallelize.

* Perform speculative coloring in parallel and then resolve coloring conflicts afterwards.

* Within a sequential speculative coloring algorithm, we could parallelize the color speculation where we get the lowest viable color for each vertex.

* Separate our graph into independent sets and operate on these sets in parallel.

### Challenges
There are several interesting challenges associated with our project.

###### Contention when parallelizing MCS
* When we search for vertices with the largest weight while parallelizing MCS, if multiple threads want to update at the same time, it could cause contention issues. May have to use locks.

###### Color conflict resolution
* When we color our graph in parallel, the color of a vertex depends on the color of its neighbors. When colors don’t update quick enough in our shared memory, it could cause coloring conflicts which will then have to be resolved afterwards.

###### Interference Graph Generation
* In our discussion with prof. Mowry, he told us that we will have to craft our own interference graphs rather than get interference graphs from compilers. This is because it is difficult to write programs that will give us large enough graphs for us to see the effect of parallelism.


#### Resources
###### Type of Computers
* We expect to use the GHC machines as well as the PSC Bridges-2 machines, similar to assignment 3 & 4. This is because we will try to perform parallelization using OpenMP, and this is a CPU intensive process that presents considerable potential for analysis when performed across multiple different cores. As compiling is generally done by the CPU, we believe that using OpenMP would demonstrate accurate real life potential for improvements.

#### Starting Point
We will begin from scratch, starting off by implementing a sequential implementation of the register allocation algorithm. Then, we can perform the parallelization as outlined in the challenges above. Source to consult for register allocation: https://www.cs.cmu.edu/~janh/courses/411/24/lectures/03-regalloc.pdf 
	

#### Goals and Deliverables
Since there is inherently sequential code in this algorithm, there exists some performance bottlenecks with parallelization. However, we hope to speed up our completely sequential implementation by log(x) times the number of threads. 

The two main metrics that we will be analyzing are speed and optimal coloring. Speed is self-explanatory in the sense that the program should speed up in a log scale proportional to the number of threads. In terms of optimal coloring, since we will be resolving coloring conflicts (either optimally or greedily), the quality of our coloring should be assessed. 

In order to demonstrate that we did a good job, we will be displaying speedup graphs and the number of colors (akin to cost). 

#### Platform Choice
The platform we have chosen to use is the C++ programming language on the GHC and PSC Bridges-2 machine. As mentioned briefly in the Resources section above, we believe that OpenMP is the optimal choice for parallelism in a CPU; the sequential code given C++, unlike a Domain-Specific Language, is performant and complete.


#### Schedule
4.01.2024 - 4.07.2024
Do literature review on color conflict resolution and finish implementing sequential version of register allocation in c++

4.08.2024 - 4.14.2024
Finish all parallel implementation for MCS

4.15.2024 - 4.21.2024
Finish all parallel implementation for greedy coloring

4.22.2024 - 4.28.2024
Finish performance measurements and tuning

4.29.2024 - 5.05.2024
Finish all work related to our final report and poster
