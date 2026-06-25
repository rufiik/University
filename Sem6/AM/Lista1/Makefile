CXX = g++
CXXFLAGS = -O2 -std=c++17 -fopenmp

all: main prim

main: main.cpp functions.hpp
	$(CXX) $(CXXFLAGS) main.cpp -o main

prim: MST.cpp Prim.hpp functions.hpp
	$(CXX) $(CXXFLAGS) MST.cpp -o mst

clean:
	rm -f main mst