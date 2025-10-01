CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

all: main main2 mstgenerator BinomialHeap

main: main.cpp 
	$(CXX) $(CXXFLAGS) -o main main.cpp 

main2: main2.cpp 
	$(CXX) $(CXXFLAGS) -o main2 main2.cpp 

mstgenerator: mstgenerator.cpp 
	$(CXX) $(CXXFLAGS) -o mstgenerator mstgenerator.cpp 
BinomialHeap: BinomialHeap.cpp
	$(CXX) $(CXXFLAGS) -o BinomialHeap BinomialHeap.cpp

clean:
	rm -f *.o main main2 mstgenerator BinomialHeap

.PHONY: all clean