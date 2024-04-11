CXX=clang++

CXXFLAGS=-Wall -g -std=c++17

MAIN=serial

SRCS=graph.cpp main.cpp

HEADERS=color.h graph.h mcs.h

OBJS=$(SRCS:.cpp=.o)

$(MAIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(MAIN)

