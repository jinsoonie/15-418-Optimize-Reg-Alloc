Project Proposal: https://docs.google.com/document/d/1V1c7jPHFIxEf27Ooir0F8c-dJipUWhVMaW7YpYMVSHI/edit?usp=sharing

Project Milestone: https://docs.google.com/document/d/1B60vDWCO8wZI-mO-EAs_pwEtlz6zeIRzjabUGWgG1vU/edit?usp=sharing

# Parallelization of Large Scale Graph Coloring
by Kevin Huang and Rex Kim
### Summary
We are going to implement various parallelizations of graph-coloring on larger scale graphs, where graph-coloring is most oftentimes used in register allocation algorithms in the compiler pipeline on multi-core CPU platforms. In parallelizing graph-coloring on larger scale graphs, we will perform a detailed analysis comparing our performance to a sequential baseline implementation. We will try to parallelize across MCS, Greedy Coloring utilizing OpenMP and OpenMPI.

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

###### Utilizing OpenMP and MPI to parallelize
* While performing the parallelization of coloring certain independent sets, there is a problem with race conditions that arises especially when utilizing OpenMP. Two nodes that are neighbors but were assigned to different threads may be updated to be the same color when they were not supposed to as a result.

###### Contention when parallelizing MCS
* When searching for vertices with the largest weight while parallelizing MCS, if multiple threads want to update at the same time, contention issues arise. Locks may be a viable option to reduce such contention.

### Schedule (as of Milestone Report)
#### 4.16.2024 - 4.21.2024
Parallel implementation for MCS using OpenMP

#### 4.22.2024 - 4.24.2024
Parallel implementation for MCS using OpenMPI

#### 4.25.2024 - 4.28.2024
Parallel implementation for Greedy Coloring using OpenMP

#### 4.29.2024 - 5.01.2024
Parallel implementation for Greedy Coloring using OpenMPI

#### 5.01.2024 - 5.05.2024
Fine tune (if necessary), compose final report and poster
