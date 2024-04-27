# CXX=g++-13
CXX=clang++
CXXFLAGS=-Wall -g -std=c++17 -fopenmp
LDFLAGS=-fopenmp

MAIN=regalloc

SRCS=graph.cpp openmp-v1.cpp sequential.cpp main.cpp
HEADERS=color.h graph.h mcs.h timer.h
OBJS=$(SRCS:.cpp=.o)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJS) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAIN)
