CXX = g++
CXXFLAGS = -std=c++17 -O2

all: randomTSP Prim

randomTSP: randomTSP.cpp functions.hpp
	$(CXX) $(CXXFLAGS) -o randomTSP randomTSP.cpp

Prim: Prim.cpp Prim.hpp functions.hpp
	$(CXX) $(CXXFLAGS) -o Prim Prim.cpp

clean:
	rm -f randomTSP Prim